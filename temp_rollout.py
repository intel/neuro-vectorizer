'''
temporary rollout file.
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
import argparse
import ray
import ray.tune as tune
from ray.rllib.agents import ppo
from envs.neurovec import NeuroVectorizerEnv
import sys
from ray.tune.registry import register_env
from ray.tune.logger import TBXLogger

parser = argparse.ArgumentParser(description='given checkpoint and dir, run inference/rollout on the files in the dir.')
parser.add_argument(
    "checkpoint", type=str, help="Checkpoint from which to roll out.")
parser.add_argument(
    "--rollout_dir",
    type=str,
    required=True,
    help = "the dir of files to rollout/inference")
parser.add_argument(
    "--new_train_data",
    default=False, 
    action='store_true',
    help = "inferencing on new training data that looks different from provided train data.")
parser.add_argument(
    "--compile",
    default=False,
    action='store_true',
    help = "compile the programs after injecting pragmas and get execution time.")
args = parser.parse_args()     
    
ray.init()
register_env("autovec", lambda config:NeuroVectorizerEnv(config))

tune.run("PPO",
        restore= args.checkpoint, 
        checkpoint_freq  = 1,
        name = "neurovectorizer_rollout",
        stop = {"episodes_total": 100000},
        config={
            "sample_batch_size": 100000,
            "train_batch_size": 100000,
            "sgd_minibatch_size": 100000,
            "env": "autovec",
            "horizon":  1,
            "num_gpus": 0,
            "model":{'fcnet_hiddens':[128,128]},
            "num_workers": 1,
            "env_config":{'dirpath':args.rollout_dir,'new_rundir':'./new_rollout_garbage',
                         'inference_mode':True, 'new_train_data':args.new_train_data,
                         'compile':args.compile}
            },
        loggers=[TBXLogger]
)
