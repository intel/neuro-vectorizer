'''
Copyright (c) 2019, Ameer Haj Ali (UC Berkeley), and Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''
import gym
from gym import spaces
import pickle
import numpy as np
import re
import os
from extractor_c import CExtractor
#from config import Config
#from my_model import Code2VecModel
#from path_context_reader import EstimatorAction
from utility import get_bruteforce_runtimes, get_O3_runtimes, get_snapshot_from_code, get_runtime, get_vectorized_codes,c_code2vec_get_encodings
#the number maximum number of leafs in the AST tree for code2vec
MAX_LEAF_NODES = 320



pragma_line = '#pragma clang loop vectorize_width({0}) interleave_count({1})\n'


#NeuroVectorizer RL Environment
class NeuroVectorizerEnv(gym.Env):
    def __init__(self, env_config):
        self.dirpath = env_config.get('dirpath')
        self.new_rundir = env_config.get('new_rundir')
        self.train_code2vec = env_config.get('train_code2vec',True)
        self.inference_mode = env_config.get('inference_mode', False)# whether or not in inference mode
        self.compile = env_config.get('compile', True) #whether to compile the progarms or not, generally turned off in inference mode when it is not clear how to compile(e.g., requires make)
        if not os.path.exists(self.new_rundir):
            print('creating '+self.new_rundir+' directory')
            os.mkdir(self.new_rundir)

        cmd = 'cp -r ' +self.dirpath+'/* ' +self.new_rundir
        print('running:',cmd)
        os.system(cmd)

        self.vec_action_meaning = [1,2,4,8,16,32,64] # TODO: change this to match your hardware
        self.interleave_action_meaning=[1,2,4,8,16] # TODO: change this to match your hardware
        self.action_space = spaces.Tuple([spaces.Discrete(len(self.vec_action_meaning)),spaces.Discrete(len(self.interleave_action_meaning))])

        self.testfiles = [os.path.join(root, name)
             for root, dirs, files in os.walk(self.new_rundir)
             for name in files
             if name.endswith(".c") and not name.startswith('header.c')]
        self.new_testfiles = list(self.testfiles) # copy testfiles
        self.loops_idxs_in_orig,self.pragmas_idxs,self.const_new_codes,self.num_loops,self.const_orig_codes=get_vectorized_codes(self.testfiles,self.new_testfiles)
        self.new_testfiles = list(self.pragmas_idxs.keys()) # to operate on files that actually have for loops
        self.current_file_idx = 0
        self.current_pragma_idx = 0
        if not self.train_code2vec: # if you want to train on new data with pretrained code2vec or other code embedding without pregathered execution times
            self.obs_len = 384 # TODO: change obs_len based on your seting in code2vec or other code embedding 
            self.observation_space = spaces.Box(-1.0,1.0,shape=(self.obs_len,),dtype = np.float32)
            self.obs_encodings = c_code2vec_get_encodings(self.new_rundir,self.const_orig_codes,self.loops_idxs_in_orig)# TODO:change this to other code embedding if necessary 
            # this should be removed in later versions    
            self.vec_action_meaning = [1,2,4,8,16] # TODO: change this to match your hardware
            self.interleave_action_meaning=[1,2,4,8] # TODO: change this to match your hardware
            self.action_space = spaces.Tuple([spaces.Discrete(len(self.vec_action_meaning)),spaces.Discrete(len(self.interleave_action_meaning))])
        else:
            from config import Config
            from my_model import Code2VecModel
            from path_context_reader import EstimatorAction
            self.obs_encodings = {}
            self.config = Config(set_defaults=True, load_from_args=False, verify=True)
            self.code2vec = Code2VecModel(self.config)
            self.path_extractor = CExtractor(self.config,clang_path=os.environ['CLANG_PATH'],max_leaves=MAX_LEAF_NODES)
             #TODO: you might need to next line based on the size of your C code, max sure to replace 10000.0 with the highest value the parser generates
            self.observation_space = spaces.Tuple([spaces.Box(0,10000,shape=(self.config.MAX_CONTEXTS,),dtype = np.int32,)]*3+[spaces.Box(0,10000.0,shape=(self.config.MAX_CONTEXTS,),dtype = np.float32)])
            self.train_input_reader = self.code2vec._create_data_reader(estimator_action=EstimatorAction.Train)
        if self.compile:
            self.O3_runtimes=get_O3_runtimes(self.new_rundir,self.new_testfiles,self.vec_action_meaning,self.interleave_action_meaning)
    
    #calculates the RL agent's reward
    def get_reward(self,new_code,current_filename):
        f = open(current_filename,'w')
        f.write(''.join(new_code))
        f.close()
        if self.compile:
            runtime = get_runtime(self.new_rundir,new_code,current_filename)
            reward = (self.O3_runtimes[current_filename]-runtime)/self.O3_runtimes[current_filename]
            if self.inference_mode and self.current_pragma_idx+1 == self.num_loops[current_filename]: # in inference mode and finished inserting pragmas to this file
                print('benchmark: ',current_filename,'O3 runtime: ', self.O3_runtimes[current_filename], 'RL runtime: ', runtime)
        else:
            reward = 0 # can't calculate the reward without runtime
        return reward

    # RL reset function
    def reset(self):
        current_filename = self.new_testfiles[self.current_file_idx]
        if self.current_pragma_idx == 0 or not self.inference_mode: #this make sure that all RL pragmas remain in the code when inferencing
            self.new_code = list(self.const_new_codes[current_filename])
        return self.get_obs(current_filename)

    # given a file returns the RL observation
    # TODO: change this if you want other embeddings
    def get_obs(self,current_filename,current_pragma_idx):
        if not self.train_code2vec:
            return self.obs_encodings[current_filename][current_pragma_idx]
        
        #check if this encoding already exists
        try:
            return self.obs_encodings[current_filename][current_pragma_idx]
        except:
            pass
        
        #to get code for files not in the dataset
        #code=get_snapshot_from_code(self.const_orig_codes[current_filename],self.loops_idxs_in_orig[current_filename][current_pragma_idx])
        code=get_snapshot_from_code(self.const_orig_codes[current_filename])
        input_full_path_filename=os.path.join(self.new_rundir,'my_code.c')
        loop_file=open(input_full_path_filename,'w')
        loop_file.write(''.join(code))
        loop_file.close()
        try:
            train_lines, hash_to_string_dict = self.path_extractor.extract_paths(input_full_path_filename)
        except:
            print('Could not parse file',current_filename, 'loop index',current_pragma_idx,'. Try removing it.')
            raise 
        dataset  = self.train_input_reader.process_and_iterate_input_from_data_lines(train_lines)
        obs = []
        tensors = list(dataset)[0][0]
        import tensorflow as tf
        for tensor in tensors:
            with tf.compat.v1.Session() as sess: 
                sess.run(tf.compat.v1.tables_initializer())
                obs.append(tf.squeeze(tensor).eval())

        if current_filename not in self.obs_encodings:
            self.obs_encodings[current_filename] = {}
        self.obs_encodings[current_filename][current_pragma_idx] = obs
        return obs

    # RL step function 
    def step(self,action):
        done = True # horizon = 1 
        action = list(np.reshape(np.array(action),(np.array(action).shape[0],)))
        VF_idx = action[0]
        IF_idx = action[1]
        VF = self.vec_action_meaning[VF_idx]
        IF = self.interleave_action_meaning[IF_idx]
        current_filename = self.new_testfiles[self.current_file_idx]
        self.new_code[self.pragmas_idxs[current_filename][self.current_pragma_idx]] = pragma_line.format(VF,IF)
        reward = self.get_reward(self.new_code,current_filename)
        #print("VF",VF,"IF",IF)
        #print('reward:', reward, 'O3',self.O3_runtimes[current_filename])
        self.current_pragma_idx += 1
        if self.current_pragma_idx == self.num_loops[current_filename]:
            #done = True
            self.current_pragma_idx=0
            self.current_file_idx += 1
            if self.current_file_idx == len(self.new_testfiles):
                self.current_file_idx = 0
                if self.inference_mode:
                    print('exiting after inferencing all programs')
                    exit(0) # finished all program/!
                
            if not self.train_code2vec:
                obs =[0]*self.obs_len
            else:
                obs = [[0]*200]*4
        else:
            obs = self.get_obs(current_filename,self.current_pragma_idx)
            #done = False
        
        return obs,reward,done,{}
