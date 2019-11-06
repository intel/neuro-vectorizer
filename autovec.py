import ray
import ray.tune as tune
from ray.rllib.agents import ppo
from envs.new_autovec import NeuroVectorizerEnv
from my_model import Code2VecModel
from ray.rllib.models import ModelCatalog
from ray.tune.registry import register_env
#agent = BanditEnv({'filepath':'/Users/ameerh/Berkeley_Drive/PhD_berkeley/llvm-project/build/rlvectorizer/llvm-test.c'})
#encodings = agent.get_encodings()
#print(encodings)
#print(len(encodings))
#print(len(encodings[0]))
ray.init()
ModelCatalog.register_custom_model("my_model",Code2VecModel)
register_env("autovec", lambda config:NeuroVectorizerEnv(config))
'''
trainer = ppo.PPOTrainer(env="autovec", config={
        "model": {
            "custom_model": "my_model",
        },
        "env_config":{'dirpath':'./training_data','new_rundir':'./new_garbage'},

    })
'''
tune.run_experiments({
    "my_experiment": {
        #"restore": "/Users/ameerh/Berkeley_Drive/PhD_berkeley/llvm-project/build/rlvectorizer/PPO_BanditEnv_0_2019-08-03_01-10-43lnjy3yyo/checkpoint_240/checkpoint-240",
        "checkpoint_freq":1,
        "run": "PPO",
        "env": NeuroVectorizerEnv,
        "stop": {"episodes_total": 50000},
        "config": {
            #"observation_filter": "NoFilter",
            "sample_batch_size": 10,
 #           "eager":True,
            "train_batch_size": 10,
            "sgd_minibatch_size": 10,
            #"num_sgd_iter":3,
            #"lr":5e-5,
            #"vf_loss_coeff":0.5,
            "horizon":  1,#tune.grid_search([15,25]),
            "num_gpus": 0,
            "model":{"custom_model": "my_model"},#tune.grid_search([{"fcnet_hiddens": [32,32]},{"fcnet_hiddens": [32]},{"fcnet_hiddens": [64]},{"fcnet_hiddens": [64,64]},{"fcnet_hiddens": [128]},{"fcnet_hiddens": [128,128]}]),
            "num_workers": 1,
            "env_config":{'dirpath':'./training_data','new_rundir':'./new_garbage'}
            },
    },
})
