
import gym
import os
from extractor_c import CExtractor
from common import common
from config import Config
SHOW_TOP_CONTEXTS=10
from path_context_reader import EstimatorAction
MAX_LEAF_NODES=320
from keras_model import Code2VecModel
class Code2VecEnv(gym.Env):
    def __init__(self, env_config):
        self.config = Config(set_defaults=True, load_from_args=True, verify=True)
        #self.action_space = <gym.Space>
        #self.observation_space = <gym.Space>
        self.path_extractor = CExtractor(self.config,clang_path=os.environ['CLANG_PATH'],max_leaves=MAX_LEAF_NODES)
    def reset(self):
        input_full_path_filename='./my_code.c'
        predict_lines, hash_to_string_dict = self.path_extractor.extract_paths(input_full_path_filename)
        self.code2vec = Code2VecModel(self.config)
        predict_input_reader = self.code2vec._create_data_reader(estimator_action=EstimatorAction.Predict)
        input_iterator = predict_input_reader.process_and_iterate_input_from_data_lines(predict_lines)
        all_model_prediction_results = []
            # perform the actual prediction and get raw results.
        input_for_predict = input_row[0][0][:4]  # we want only the relevant input vectors (w.o. the targets).
        return input_for_predict    
        #return <obs>
    #def step(self, action):
        #return <obs>, <reward: float>, <done: bool>, <info: dict>
Code2VecEnv({})
 

