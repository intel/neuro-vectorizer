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
import os
import numpy as np

# operates on pretrained code2vec with pregathered data
class SimNeuroVectorizerEnv(gym.Env):
    def __init__(self,env_config):
        self.continuous = env_config.get('action_type',0)
        rundir = env_config.get('rundir')
        self.vec_action_meaning = [1,2,4,8,16] #TODO: modify based on the architecture
        self.interleave_action_meaning=[1,2,4,8] # TODO: modify based on the architecture
        self.action_space = spaces.Tuple([spaces.Discrete(len(self.vec_action_meaning)),spaces.Discrete(len(self.interleave_action_meaning))])
        if(self.continuous == 1):
            self.action_space = spaces.Box(0.0,1.0,shape=(1,),dtype=np.float32)
        if(self.continuous == 2):
            self.action_space = spaces.Box(0.0,1.0,shape=(2,),dtype=np.float32)
        self.obs_len = 384 # TODO: change obs_len based on your seting in code2vec

        self.observation_space = spaces.Box(-1.0,1.0,shape=(self.obs_len,),dtype = np.float32)
        with open(os.path.join(rundir,'dataset.pkl'), 'rb') as f: # can be omitted
            data =  pickle.load(f)
            self.runtimes = data['all_program_runtimes']
            opt_runtimes = data['opt_runtimes']
        with open(os.path.join(rundir,'O3_runtimes.pkl'),'rb') as f: # O3 runtimes
            self.o3_runtimes = pickle.load(f)
        with open(os.path.join(rundir,'c_encodings.pkl'),'rb') as f: # encodings
            self.encodings = pickle.load(f)
        self.rollout = env_config.get('rollout',False)
        self.testkeys =[]
        self.get_runtimes(opt_runtimes,self.rollout)
        self.current_file_idx = 0
            
    def get_runtimes(self,opt_runtimes,rollout=False):
        if not rollout:
            for key in opt_runtimes.keys():
                o3_key=key.replace('_74296','_22506')
                o3 = self.o3_runtimes[o3_key]
                opt = opt_runtimes[key]
                improv = (o3-opt)*1.0/o3
                if improv >0.01:
                    self.testkeys.append(key)
        else:
            self.testkeys=['garbage24/s6n_64_add_4_74296.c', 'garbage24/s2_4096_2_2_x_out_74296.c', 'garbage24/s12_128_2_2_input_assign_74296.c', 'garbage24/s12n_256_2_2_in1_assign1_74296.c', 'garbage24/s9n_64_64_2_x_74296.c', 'garbage24/s12nn_64_2_2_short_a_assign1_74296.c', 'garbage24/s7n_16384_3_74296.c', 'garbage24/s11_256_mul_4_74296.c', 'garbage24/s13_512_sub_1_74296.c', 'garbage24/s10_8192_add_4_74296.c', 'garbage24/s8_256_4_74296.c', 'garbage24/s7_2048_2_74296.c']

    def reset(self):
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
            action = round(action[0]*(len(self.vec_action_meaning)*len(self.interleave_action_meaning)-1))
            VF_idx = int(int(action)/len(self.interleave_action_meaning))
            IF_idx = int(int(action)%len(self.interleave_action_meaning))
        if self.continuous == 2:
            VF_idx = int(round(action[0]*(len(self.vec_action_meaning)-1)))
            IF_idx = int(round(action[1]*(len(self.interleave_action_meaning)-1)))

        return VF_idx, IF_idx
    def step(self,action):
        VF_idx, IF_idx = self.get_VF_IF(action)
        reward,O3_time,exec_time = self.get_reward(VF_idx,IF_idx)

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
        info = {}
        if self.rollout:
            info = {'O3':O3_time,'RL':exec_time,'VF':self.vec_action_meaning[VF_idx],'IF':self.interleave_action_meaning[IF_idx]}
        return obs, reward, True, info

    def get_reward(self,VF_idx,IF_idx):
        key = self.testkeys[self.current_file_idx]
        o3_key = key.replace('_74296','_22506')
        o3 = self.o3_runtimes[o3_key]
        exec_time = self.runtimes[key][VF_idx][IF_idx]
        if(exec_time>=1000000):
            reward= -9
        else:
            reward = (o3-exec_time)*1.0/o3
        return reward,o3,exec_time

