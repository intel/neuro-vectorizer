import gym
from gym import spaces
import pickle
import numpy as np
import re
import os
from extractor_c import CExtractor
from common import common
from config import Config
from my_model import Code2VecModel
from path_context_reader import EstimatorAction
from utility import get_bruteforce_runtimes, get_O3_runtimes, get_snapshot_from_code, get_runtime
#the number maximum number of leafs in the AST tree for code2vec
MAX_LEAF_NODES = 320



pragma_line = '#pragma clang loop vectorize_width({0}) interleave_count({1})\n'

def get_block(i,code):
    j = i
    cnt = 0
    while(True):
        line = code[j]
        if re.match(r'^\s*//',line) or re.match(r'^\s*$',line):
            j += 1
            continue
        if '{' in line:
            cnt += line.count('{')
        if '}' in line:
            cnt -= line.count('}')
        if cnt == 0 and not (re.match(r'^\s*for\s*\(',line) or re.match(r'^\s*while\s*\(',line)):
            return (i,j)
        if cnt == 0 and line.endswith(';\n'):
            return (i,j)
        if (re.match(r'^\s*for\s*\(',line) or re.match(r'^\s*while\s*\(',line)) and i != j:
            return get_block(j,code)
        j=j+1

def get_vectorized_code(code):
    new_code = []
    for_loops_indices = []
    i=0
    pragma_indices = []
    num_elems_in_new_code=0
    while i < len(code):
        line=code[i]
        if re.match(r'^\s*for\s*\(',line) or re.match(r'^\s*while\s*\(',line):
            begining,ending = get_block(i,code)
            orig_i=i
            while(i<ending+1):
                if i==begining:
                    new_code.append('//'+pragma_line.format(64,16))#start with -O3 vectorization
                    num_elems_in_new_code += 1
                    pragma_indices.append(num_elems_in_new_code-1)
                new_code.append(code[i])
                num_elems_in_new_code += 1
                i = i+1
            # to pick the index of the most innner loop    
            #for_loops_indices.append((orig_i,ending))
            for_loops_indices.append((begining,ending))
            i=ending+1
            continue
        new_code.append(line)
        num_elems_in_new_code += 1
        i += 1

    return for_loops_indices,pragma_indices,new_code

def get_vectorized_codes(testfiles, new_testfiles):
    loops_idxs_in_orig = {}
    pragmas_idxs = {}
    const_new_codes ={}
    num_loops = {}
    const_orig_codes={}
    for o_fn,n_fn in zip(testfiles,new_testfiles):
        f = open(o_fn,'r')
        try:
            code = f.readlines()
        except:
            f.close()
            continue

        loops_idx, pragmas_idx, new_code = get_vectorized_code(code)
        if not pragmas_idx:
            f.close()
            continue
        const_orig_codes[n_fn] = list(code)
        loops_idxs_in_orig[n_fn]=list(loops_idx)
        pragmas_idxs[n_fn] = list(pragmas_idx)
        const_new_codes[n_fn] = list(new_code)
        num_loops[n_fn] = len(pragmas_idx)
        print('writing file...',n_fn)
        nf = open(n_fn,'w')
        nf.write(''.join(new_code))
        nf.close()
        f.close()
    return loops_idxs_in_orig, pragmas_idxs, const_new_codes,num_loops,const_orig_codes

#NeuroVectorizer RL Environment
class NeuroVectorizerEnv(gym.Env):
    def __init__(self, env_config):
        self.config = Config(set_defaults=True, load_from_args=False, verify=True)
        self.code2vec = Code2VecModel(self.config)
        self.path_extractor = CExtractor(self.config,clang_path=os.environ['CLANG_PATH'],max_leaves=MAX_LEAF_NODES)
        self.dirpath = env_config.get('dirpath')
        self.new_rundir = env_config.get('new_rundir')
        cmd = 'rm -r ' +self.new_rundir
        print(cmd)
        os.system(cmd)
        if not os.path.isdir(self.new_rundir):
            print('creating '+self.new_rundir+' directory')
            os.mkdir(self.new_rundir)
            cmd = 'cp -r ' +self.dirpath+'/* ' +self.new_rundir
            os.system(cmd)
        self.vec_action_meaning = [1,2,4,8,16,32,64] # change this to match your hardware
        self.interleave_action_meaning=[1,2,4,8,16] # change this to match your hardware
        self.action_space = spaces.Discrete(len(self.vec_action_meaning)*len(self.interleave_action_meaning))
        self.observation_space = spaces.Tuple([spaces.Box(0,10000,shape=(self.config.MAX_CONTEXTS,),dtype = np.int32,)]*3+[spaces.Box(0,10000.0,shape=(self.config.MAX_CONTEXTS,),dtype = np.float32)]) # you might need to change this based on the size of your C code, max sure to replace 10000.0 with the highest value the parser generates
        self.testfiles = [os.path.join(root, name)
             for root, dirs, files in os.walk(self.new_rundir)
             for name in files
             if name.endswith(".c") and not name.startswith('header.c')]
        self.new_testfiles = list(self.testfiles) # copy testfiles
        self.loops_idxs_in_orig,self.pragmas_idxs,self.const_new_codes,self.num_loops,self.const_orig_codes=get_vectorized_codes(self.testfiles,self.new_testfiles)
        self.new_testfiles = list(self.pragmas_idxs.keys()) # to operate on files that actually have for loops
        self.current_file_idx = 0
        self.current_pragma_idx = 0
        self.train_input_reader = self.code2vec._create_data_reader(estimator_action=EstimatorAction.Train)
        self.O3_runtimes=get_O3_runtimes(self.new_rundir,self.new_testfiles,self.vec_action_meaning,self.interleave_action_meaning)
    

    #calculates the RL agent's reward
    def get_reward(self,new_code,current_filename):
        runtime = get_runtime(self.new_rundir,new_code,current_filename)
        reward = (self.O3_runtimes[current_filename]-runtime)/self.O3_runtimes[current_filename]
        return reward

    # RL reset function
    def reset(self):
        current_filename = self.new_testfiles[self.current_file_idx]
        self.new_code = list(self.const_new_codes[current_filename])
        return self.get_obs(current_filename)

    # given a file returns the RL observation
    def get_obs(self,current_filename):
        #to get code for files not in the dataset
        #code=get_snapshot_from_code(self.const_orig_codes[current_filename],self.loops_idxs_in_orig[current_filename][self.current_pragma_idx])
        code=get_snapshot_from_code(self.const_orig_codes[current_filename])
        input_full_path_filename=os.path.join(self.new_rundir,'my_code.c')
        loop_file=open(input_full_path_filename,'w')
        loop_file.write(''.join(code))
        loop_file.close()
        train_lines, hash_to_string_dict = self.path_extractor.extract_paths(input_full_path_filename)
        dataset  = self.train_input_reader.process_and_iterate_input_from_data_lines(train_lines)
        obs = []
        tensors = list(dataset)[0][0]
        import tensorflow as tf
        for tensor in tensors:
            with tf.compat.v1.Session() as sess: 
                sess.run(tf.compat.v1.tables_initializer())
                obs.append(tf.squeeze(tensor).eval())
        return obs

    # RL step function 
    def step(self,action):
        done = True # horizon = 1 
        VF_idx = int(int(action)/len(self.interleave_action_meaning))
        IF_idx = int(int(action)%len(self.interleave_action_meaning))
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
            obs = [[0]*200]*4
        else:
            obs = self.get_obs(current_filename)
            #done = False
        
        return obs,reward,done,{}
