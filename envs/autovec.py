import copy
import gym
from gym import spaces
import pickle
import time
import numpy as np
import re
import subprocess
import os
import traceback
import hashlib
from extractor_c import CExtractor
from common import common
from common import Config, VocabType
from model import Model

SHOW_TOP_CONTEXTS = 10
MAX_LEAF_NODES = 320
#from vocabularies import VocabType
#from config import Config
#from interactive_predict import InteractivePredictor
#from model_base import Code2VecModelBase
from argparse import ArgumentParser
import sys
import os


#from extractor import Extractor
'''
SHOW_TOP_CONTEXTS = 10
MAX_PATH_LENGTH = 8
MAX_PATH_WIDTH = 2
# this next line should be modified everywhere
JAR_PATH = '/Users/ameerh/Berkeley_Drive/PhD_berkeley/llvm-project/build/rlvectorizer/envs/code2vec/JavaExtractor/JPredict/target/JavaExtractor-0.0.1-SNAPSHOT.jar'




def load_model_dynamically(config: Config) -> Code2VecModelBase:
    assert config.DL_FRAMEWORK in {'tensorflow', 'keras'}
    if config.DL_FRAMEWORK == 'tensorflow':
        from tensorflow_model import Code2VecModel
    elif config.DL_FRAMEWORK == 'keras':
        from keras_model import Code2VecModel
    return Code2VecModel(config)
'''

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
            #for_loops_indices.append((orig_i,ending))
            for_loops_indices.append((begining,ending))
            i=ending+1
            continue
        new_code.append(line)
        num_elems_in_new_code += 1
        i += 1
#    for (b,e) in for_loops:
#        for i in range(b,e+1):
#            print(code[i])
#    print('------------')
#    for line in new_code:
#        print(line)

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

class SimBanditEnv(gym.Env):
    def __init__(self,env_config):
        self.vec_action_meaning = [1,2,4,8,16]
        self.interleave_action_meaning=[1,2,4,8]
        self.action_space = spaces.Tuple([spaces.Discrete(len(self.vec_action_meaning)),spaces.Discrete(len(self.interleave_action_meaning))])
        #self.action_space = spaces.Box(0.0,1.0,shape=(2,),dtype=np.float32)
        self.obs_len = 384 
        self.continuous = env_config.get('action_type',0)
        self.observation_space = spaces.Box(-1.0,1.,shape=(self.obs_len),dtype = np.float32)
        with open('/home/ameer/Desktop/rlvectorizer/llvm-project/build/rlvectorizer/c-code2vec_embedding/dataset.pkl', 'rb') as f:
            data =  pickle.load(f)
            self.runtimes = data['all_program_runtimes']
            opt_runtimes = data['opt_runtimes']
        with open('/home/ameer/Desktop/rlvectorizer/llvm-project/build/rlvectorizer/c-code2vec_embedding/O3_runtimes.pkl','rb') as f:
            self.o3_runtimes = pickle.load(f)
        with open('/home/ameer/Desktop/rlvectorizer/llvm-project/build/rlvectorizer/c-code2vec_embedding/c_encodings.pkl','rb') as f:
            self.encodings = pickle.load(f)
        self.testkeys =[]
        self.current_file_idx = 0
#        for key in opt_runtimes.keys():
#            o3_key=key.replace('_74296','_22506')
#            o3 = self.o3_runtimes[o3_key]
#            opt = opt_runtimes[key]
#            improv = (o3-opt)*1.0/o3
#            if improv >0.01:
#                self.testkeys.append(key)
        self.testkeys=['garbage24/s6n_64_add_4_74296.c', 'garbage24/s2_4096_2_2_x_out_74296.c', 'garbage24/s12_128_2_2_input_assign_74296.c', 'garbage24/s12n_256_2_2_in1_assign1_74296.c', 'garbage24/s9n_64_64_2_x_74296.c', 'garbage24/s12nn_64_2_2_short_a_assign1_74296.c', 'garbage24/s7n_16384_3_74296.c', 'garbage24/s11_256_mul_4_74296.c', 'garbage24/s13_512_sub_1_74296.c', 'garbage24/s10_8192_add_4_74296.c', 'garbage24/s8_256_4_74296.c', 'garbage24/s7_2048_2_74296.c']
        self.geo=1
        self.datapoints = []
        print(len(self.testkeys))
    def reset(self):
        if self.current_file_idx == 0:
            self.geo=1
        e_key = self.testkeys[self.current_file_idx].replace('_74296','_27483')
        return_e = []
        for val in self.encodings[e_key][0]:
            new_val = val
            if new_val > 1:
                new_val = 1
            if new_val <-1:
                new_val = -1
            return_e.append(new_val)
        return return_e

    def get_VF_IF(self,action):
        if self.continuous == 0:
            action = list(np.reshape(np.array(action),(np.array(action).shape[0],)))
            VF_idx = action[0]
            IF_idx = action[1]
        if self.continuous == 1:
            action = round(action[0]*19.0)
            VF_idx = int(int(action)/len(self.interleave_action_meaning))
            IF_idx = int(int(action)%len(self.interleave_action_meaning))
        if self.continuous == 2:
            VF_idx = int(round(action[0]*(len(self.vec_action_meaning)-1)))
            IF_idx = int(round(action[1]*(len(self.interleave_action_meaning)-1)))

#        print(VF_idx,IF_idx)
        return VF_idx, IF_idx
    def step(self,action):
        VF_idx, IF_idx = self.get_VF_IF(action)
        reward = self.get_reward(VF_idx,IF_idx)

        #print(VF_idx,IF_idx,reward,self.current_file_idx)
        self.current_file_idx = (self.current_file_idx+1)%len(self.testkeys)
        e_key = self.testkeys[self.current_file_idx].replace('_74296','_27483')
        obs = []
        for val in self.encodings[e_key][0]:
            new_val = val
            if new_val > 1:
                new_val = 1
            if new_val <-1:
                new_val = -1
            obs.append(new_val)

        return obs, reward, True, {}

    def get_reward(self,VF_idx,IF_idx):
        key = self.testkeys[self.current_file_idx]
        o3_key = key.replace('_74296','_22506')
        o3 = self.o3_runtimes[o3_key]
        exec_time = self.runtimes[key][VF_idx][IF_idx]
        if(exec_time>=1000000):
            reward= -9
        else:
            reward = (o3-exec_time)*1.0/o3
            self.geo=self.geo*((o3/exec_time)**(1/12))
            #if(self.geo>2.65):
            #    print('improvement',o3/exec_time,'Factors',VF_idx,IF_idx,'geo',self.geo)
            #    print('@@@@@@@@@@@@@@@@@@@@')
        return reward

class BanditEnv(gym.Env):
    def __init__(self, env_config):
        #self.dirpath='/Users/ameerh/Berkeley_Drive/PhD_berkeley/llvm-project/build/rlvectorizer/pollybench'
        self.dirpath = env_config.get('dirpath')
        self.new_rundir = env_config.get('new_rundir')
        if not os.path.isdir(self.new_rundir):
            print('creating '+self.new_rundir+' directory')
            os.mkdir(self.new_rundir)
            cmd = 'cp -r ' +self.dirpath+'/* ' +self.new_rundir
            print(cmd)
            os.system(cmd)
        self.vec_action_meaning = [1,2,4,8,16,32,64]
        self.interleave_action_meaning=[1,2,4,8,16]
        self.action_space = spaces.Tuple([spaces.Discrete(len(self.vec_action_meaning)),spaces.Discrete(len(self.interleave_action_meaning))])
        self.obs_len = 384
        self.observation_space = spaces.Box(-1.0,1.0,shape=(self.obs_len,),dtype = np.float32)
        #self.testfiles = [self.dirpath+'/'+f for f in files if f.endswith(".c")]# and f.startswith('s')]
        self.testfiles = [os.path.join(root, name)
             for root, dirs, files in os.walk(self.new_rundir)
             for name in files
             if name.endswith(".c") and not name.startswith('header.c')]
        #self.testfiles = [self.new_rundir+'/'+f for f in os.walk(self.new_rundir) if f.endswith(".c")# and f.startswith('s')]
        #self.new_testfiles = [self.new_rundir + '/'+f.split('/')[-1][:-2]+'_'+str(os.getpid())+'.c' for f in self.testfiles]
        self.new_testfiles = list(self.testfiles)#[self.new_rundir + '/'+f.split('/')[-1] for f in self.testfiles]
        print(self.new_testfiles)
        self.loops_idxs_in_orig,self.pragmas_idxs,self.const_new_codes,self.num_loops,self.const_orig_codes=get_vectorized_codes(self.testfiles,self.new_testfiles)
        self.new_testfiles = list(self.pragmas_idxs.keys())
        self.current_file_idx = 0
        self.current_pragma_idx = 0
        self.stop_inference = False
        #self.obs_encodings = env_config.get('encodings')#self.get_encodings()
        #with open('/Users/ameerh/Berkeley_Drive/PhD_berkeley/llvm-project/build/rlvectorizer/c_encodings.pkl','rb') as f:
        #    self.obs_encodings = pickle.load(f)
        #print(self.obs_encodings)
        #self.O3_runtimes = self.get_O3_runtimes()
        #print(self.O3_runtimes)

    def get_runtimes_dir(self):
        opt_runtimes = {}
        opt_factors = {}
        all_program_runtimes = {}
        one_program_runtimes = [[0]*len(self.interleave_action_meaning) for vf in range(len(self.vec_action_meaning))]
        full_path_header = os.path.join(self.new_rundir,'header.c')
        for filename in self.new_testfiles:
            opt_runtime = 1e+9
            opt_factor = (1,1)
            for i,VF in enumerate(self.vec_action_meaning):
                for j,IF in enumerate(self.interleave_action_meaning):
                    rm_cmd = 'rm ' + filename[:-1]+'o '
                    os.system(rm_cmd)
                    cmd1 = 'timeout 4s /usr/bin/clang -O3 '+full_path_header +' ' +filename+' -Rpass=loop-vectorize -mllvm -force-vector-width='+str(VF)+' -mllvm -force-vector-interleave='+str(IF)+' -o ' +filename[:-1]+'o'# TODO: fix path
                    print(cmd1)
                    os.system(cmd1)
                    cmd2 = filename[:-1]+'o '
                    try:
                        runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
                    except:
                        runtime = 1000000 #inf if fails replace with 5 times O3 runtimes
                    print(runtime)
                    print(opt_runtime)
                    print(opt_factor)
                    one_program_runtimes[i][j] = runtime
                    if runtime<opt_runtime:
                        opt_runtime = runtime
                        opt_factor = (VF,IF)
            opt_runtimes[filename] = opt_runtime
            opt_factors[filename] = opt_factor
            all_program_runtimes[filename]=copy.deepcopy(one_program_runtimes)
        print(opt_runtimes)
        print(opt_factors)
        print(all_program_runtimes)
        data={'opt_runtimes':opt_runtimes,'opt_factors':opt_factors,'all_program_runtimes':all_program_runtimes}
        output = open(os.path.join(self.new_rundir,'dataset.pkl'), 'wb')
        pickle.dump(data, output)
        output.close()

    def get_O3_runtimes(self):
        O3_runtimes={}
        full_path_header = os.path.join(self.new_rundir,'header.c')
        for filename in self.new_testfiles:
            rm_cmd = 'rm ' + filename[:-1]+'o '
            os.system(rm_cmd)
            cmd1 = 'timeout 2s /usr/bin/clang -O3 '+full_path_header +' ' +filename+' -o ' +filename[:-1]+'o'# TODO: fix path
            print(cmd1)
            os.system(cmd1)
            cmd2 = filename[:-1]+'o '
            try:
                runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
            except:
                runtime = 1000000 #inf if fails replace with 5 times O3 runtimes
            O3_runtimes[filename]=runtime

        output = open(os.path.join(self.new_rundir,'O3_runtimes.pkl'), 'wb')
        pickle.dump(O3_runtimes, output)
        output.close()
        return O3_runtimes

    def get_snapshot_from_code(self,code,loop_idx=None):
        new_code =[]
        if loop_idx:
            new_code.append('__attribute__((noinline))\n')
            new_code.append('void example() {\n')
            new_code.extend(code[loop_idx[0]:loop_idx[1]+1])
            new_code.extend('}\n')
            print(new_code)
            return new_code
        found = False
        for line in code:
            if '__attribute__' in line:
                found = True
            if 'int main(' in line:
                break
            if found:
                new_code.append(line)
        return new_code

    def c_code2vec_get_encodings(self):
        encodings={}
        #parser = ArgumentParser()
        #args = parser.parse_args()
        config = Config.get_default_config()
        self.model = Model(config)
        print('created model')
        self.path_extractor = CExtractor(config,
                                         clang_path=os.environ['CLANG_PATH'],
                                         max_leaves=MAX_LEAF_NODES)
        directory=self.new_rundir
        input_full_path_filename = os.path.join(directory, 'c_code2vec_input.c')
        print(input_full_path_filename)
        for key in self.const_orig_codes.keys():
            encodings[key] = {}
            for idx,loop_idx in enumerate(self.loops_idxs_in_orig[key]):
                ## work around ##
                code = self.get_snapshot_from_code(self.const_orig_codes[key],loop_idx)
                ## endo of work around ##
                #code = self.const_orig_codes[key][loop_idx[0]:loop_idx[1]+1]
                loop_file=open(input_full_path_filename,'w')
                loop_file.write(''.join(code))
                loop_file.close()
                predict_lines, hash_to_string_dict = self.path_extractor.extract_paths(input_full_path_filename)
                print('predict lines:',predict_lines)
                print('hash:',hash_to_string_dict)
                results, code_vectors = self.model.predict(predict_lines)
                print(sum(code_vectors[0]))
                print(code)
                encodings[key][idx] = code_vectors[0]
        self.model.close_session()
        print(encodings)
        output = open(os.path.join(self.new_rundir,'c_encodings.pkl'), 'wb')
        pickle.dump(encodings, output)
        output.close()
        return encodings

    def code2vec_get_encodings(self):

        config = Config(set_defaults=True, load_from_args=True, verify=True)
        self.model = load_model_dynamically(config)
        config.log('Done creating code2vec model')
        self.path_extractor = Extractor(config,
                                        jar_path=JAR_PATH,
                                        max_path_length=MAX_PATH_LENGTH,
                                        max_path_width=MAX_PATH_WIDTH)
        return self.manual_get_encodings()

#from already prewritten code files
    def code2vec_manual_get_encodings(self):
        encodings = []
        for prog_file_idx in range(self.num_of_loops):
            predict_lines, hash_to_string_dict = self.path_extractor.extract_paths('./java_files/'+str(prog_file_idx))
            raw_prediction_results = self.model.predict(predict_lines)
            encodings.append(raw_prediction_results[0].code_vector)
        self.model.close_session()

        data={'encodings':encodings}
        output = open(os.path.join(self.new_rundir,'encodings.pkl'), 'wb')
        pickle.dump(data, output)
        output.close()
        return encodings

#from current code file
    def automatic_get_encodings(self):
        encodings = []
        for loop_idx in self.loop_indeces_in_orig:
            code = self.original_code[loop_idx[0]:loop_idx[1]+1]
            loop_file=open('Input.java','w')
            loop_file.write(''.join(code))
            loop_file.close()
            predict_lines, hash_to_string_dict = self.path_extractor.extract_paths('Input.java')
            raw_prediction_results = self.model.predict(predict_lines)
            encodings.append(raw_prediction_results[0].code_vector)
        self.model.close_session()
        return encodings

    def run_llvm_test_shell_command(self,filename):
        cmd1 = '/usr/bin/clang -O3 header.c ' +filename+' -o ' +filename[:-1]+'o'# TODO: fix path
        cmd2 = filename[:-1]+'o '
        print(cmd1)
        os.system(cmd1)
        runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
        return runtime


    def get_runtime(self,new_code,current_filename):
        tf = open(current_filename,'w')
        tf.write(''.join(new_code))
        tf.close()
        #runtime = self.run_shell_command(filename=self.testfile_name)
        runtime=self.run_llvm_test_shell_command(current_filename)
        return runtime

    def get_reward(self,new_code,current_filename):
        runtime = self.get_runtime(new_code,current_filename)
        #reward = self.current_runtime-runtime
        #self.current_runtime=runtime
        reward = (self.O3_runtimes[current_filename]-runtime)/self.O3_runtimes[current_filename]
        return reward

    def get_fake_reward(self,test_num,VF,IF):
        reward = self.O3_runtimes[test_num]-self.O3_runtimes[test_num]/VF+IF
        return reward

    #inference_reset
    def reset(self):
        if self.stop_inference:
            print('exiting end of inference...')
            exit(0)
        current_filename = self.new_testfiles[self.current_file_idx]
        if self.current_pragma_idx == 0:
            self.new_code = list(self.const_new_codes[current_filename])
        return self.encoded_reset(current_filename)
    #inference_step
    def step(self,action):
        done = True
        action = list(np.reshape(np.array(action),(np.array(action).shape[0],)))
        VF = self.vec_action_meaning[action[0]]
        IF = self.interleave_action_meaning[action[1]]
        current_filename = self.new_testfiles[self.current_file_idx]
        self.new_code[self.pragmas_idxs[current_filename][self.current_pragma_idx]] = pragma_line.format(VF,IF)
        tf = open(current_filename,'w')
        tf.write(''.join(self.new_code))
        tf.close()

        reward = 0
        print("VF",VF,"IF",IF)
        #print('reward:', reward, 'O3',self.O3_runtimes[current_file])
        self.current_pragma_idx += 1
        obs = self.encoded_step(self.current_pragma_idx,current_filename)
        if self.current_pragma_idx == self.num_loops[current_filename]:
            self.current_pragma_idx=0
            self.current_file_idx += 1
            if self.current_file_idx == len(self.new_testfiles):
                self.current_file_idx = 0
                self.stop_inference = True
        print(sum([x*x for x in obs]))
        return obs,reward,done,{}

    def orig_reset(self):
        current_filename = self.new_testfiles[self.current_file_idx]
        self.new_code = list(self.const_new_codes[current_filename])
        return self.encoded_reset(current_filename)

    def encoded_reset(self,current_filename):
        obs=self.obs_encodings[current_filename][self.current_pragma_idx]
        return obs

    def encoded_step(self,current_pragma_idx,current_filename):
        if current_pragma_idx == self.num_loops[current_filename]:
            garbage = 0
            obs = [garbage]*self.obs_len
        else:
            obs = self.obs_encodings[current_filename][current_pragma_idx]
        return obs
    def orig_step(self,action):
        done = False
        action = list(np.reshape(np.array(action),(np.array(action).shape[0],)))
        VF = self.vec_action_meaning[action[0]]
        IF = self.interleave_action_meaning[action[1]]
        current_filename = self.new_testfiles[self.current_file_idx]
        self.new_code[self.pragmas_idxs[current_filename][self.current_pragma_idx]] = pragma_line.format(VF,IF)
        reward = self.get_reward(self.new_code,current_filename)#self.get_fake_reward(self.current_pragma_idx,VF,IF)#
        print("VF",VF,"IF",IF)
        print('reward:', reward, 'O3',self.O3_runtimes[current_file])
        self.current_pragma_idx += 1
        obs = self.encoded_step(self.current_pragma_idx,current_filename)
        if self.current_pragma_idx == self.num_loops[current_filename]:
            done = True
            self.current_pragma_idx=0
            self.current_file_idx += 1
            if self.current_file_idx == len(self.new_testfiles):
                self.current_file_idx = 0

        else:
            done = False

        return obs,reward,done,{}
