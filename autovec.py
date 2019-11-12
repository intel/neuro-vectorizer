import ray
import ray.tune as tune
from ray.rllib.agents import ppo
from envs.neurovec import NeuroVectorizerEnv
from my_model import Code2VecModel
from ray.rllib.models import ModelCatalog
from ray.tune.registry import register_env

ray.init()
ModelCatalog.register_custom_model("my_model",Code2VecModel)
register_env("autovec", lambda config:NeuroVectorizerEnv(config))

tune.run_experiments({
    "NeuroVectorizer": {
        #"restore": "/Users/ameerh/Berkeley_Drive/PhD_berkeley/llvm-project/build/rlvectorizer/PPO_BanditEnv_0_2019-08-03_01-10-43lnjy3yyo/checkpoint_240/checkpoint-240",
        "checkpoint_freq":1,
        "run": "PPO",
        "env": NeuroVectorizerEnv,
        "stop": {"episodes_total": 500000},
        "config": {
            #"observation_filter": "NoFilter",
            #"sample_batch_size": 10,
 #           "eager":True,
            #"train_batch_size": 10,
            #"sgd_minibatch_size": 10,
            #"num_sgd_iter":3,
            #"lr":5e-5,
            #"vf_loss_coeff":0.5,
            "horizon":  1,
            "num_gpus": 0,
            "model":{"custom_model": "my_model"},
            "num_workers": 1,
            "env_config":{'dirpath':'./training_data','new_rundir':'./new_garbage'}
            },
    },
})
