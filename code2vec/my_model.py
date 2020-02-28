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
from path_context_reader import PathContextReader, ModelInputTensorsFormer, ReaderInputTensors, EstimatorAction
import os
import numpy as np
from functools import partial
from typing import List, Optional, Iterable, Union, Callable, Dict
from collections import namedtuple
import time
import datetime
from vocabularies import VocabType
from keras_attention_layer import AttentionLayer
from keras_topk_word_predictions_layer import TopKWordPredictionsLayer
from keras_words_subtoken_metrics import WordsSubtokenPrecisionMetric, WordsSubtokenRecallMetric, WordsSubtokenF1Metric
from config import Config
from common import common
from neuro_model_base import Code2VecModelBase, ModelEvaluationResults, ModelPredictionResults
from keras_checkpoint_saver_callback import ModelTrainingStatus, ModelTrainingStatusTrackerCallback,\
    ModelCheckpointSaverCallback, MultiBatchCallback, ModelTrainingProgressLoggerCallback

from ray.rllib.models.tf.tf_modelv2 import TFModelV2
import ray
from ray import tune
from ray.rllib.models import ModelCatalog

class Code2VecModel(Code2VecModelBase,TFModelV2):
    def __init__(self, obs_space=None,action_space=None,num_outputs=35,model_config={},name='my_model'):
        self.base_model: Optional[keras.Model] = None
        self.keras_eval_model: Optional[keras.Model] = None
        self.keras_model_predict_function: Optional[K.GraphExecutionFunction] = None
        self.training_status: ModelTrainingStatus = ModelTrainingStatus()
        self._checkpoint: Optional[tf.train.Checkpoint] = None
        self._checkpoint_manager: Optional[tf.train.CheckpointManager] = None
        TFModelV2.__init__(self,obs_space, action_space,num_outputs, model_config, name)
        config = Config(set_defaults=True, load_from_args=True, verify=True)
        Code2VecModelBase.__init__(self,config)

#    def _create_keras_model(self):
        import tensorflow as tf
        from tensorflow import keras
        from tensorflow.keras.layers import Input, Embedding, Concatenate, Dropout, TimeDistributed, Dense
        from tensorflow.keras.callbacks import Callback
        import tensorflow.keras.backend as K
        from tensorflow.keras.metrics import sparse_top_k_categorical_accuracy
        # Each input sample consists of a bag of x`MAX_CONTEXTS` tuples (source_terminal, path, target_terminal).
        # The valid mask indicates for each context whether it actually exists or it is just a padding.
        path_source_token_input = Input((self.config.MAX_CONTEXTS,), dtype=tf.int32)
        path_input = Input((self.config.MAX_CONTEXTS,), dtype=tf.int32)
        path_target_token_input = Input((self.config.MAX_CONTEXTS,), dtype=tf.int32)
        context_valid_mask = Input((self.config.MAX_CONTEXTS,))

        # Input paths are indexes, we embed these here.
        paths_embedded = Embedding(
            self.vocabs.path_vocab.size, self.config.PATH_EMBEDDINGS_SIZE, name='path_embedding')(path_input)

        # Input terminals are indexes, we embed these here.
        token_embedding_shared_layer = Embedding(
            self.vocabs.token_vocab.size, self.config.TOKEN_EMBEDDINGS_SIZE, name='token_embedding')
        path_source_token_embedded = token_embedding_shared_layer(path_source_token_input)
        path_target_token_embedded = token_embedding_shared_layer(path_target_token_input)

        # `Context` is a concatenation of the 2 terminals & path embedding.
        # Each context is a vector of size 3 * EMBEDDINGS_SIZE.
        context_embedded = Concatenate()([path_source_token_embedded, paths_embedded, path_target_token_embedded])
        context_embedded = Dropout(1 - self.config.DROPOUT_KEEP_RATE)(context_embedded)

        # Lets get dense: Apply a dense layer for each context vector (using same weights for all of the context).
        context_after_dense = TimeDistributed(
            Dense(self.config.CODE_VECTOR_SIZE, use_bias=False, activation='tanh'))(context_embedded)

        # The final code vectors are received by applying attention to the "densed" context vectors.
        code_vectors, attention_weights = AttentionLayer(name='attention')(
            [context_after_dense, context_valid_mask])

        # "Decode": Now we use another dense layer to get the target word embedding from each code vector.
        #target_index = Dense(
        #    self.vocabs.target_vocab.size, use_bias=False, activation='softmax', name='target_index')(code_vectors)
        target_index = Dense(
            num_outputs, use_bias=False, activation='softmax', name='target_index')(code_vectors)
        value_out = Dense(1, activation=None,name='value_out')(code_vectors)
        # Wrap the layers into a Keras model, using our subtoken-metrics and the CE loss.
        inputs = [path_source_token_input, path_input, path_target_token_input, context_valid_mask]
        self.base_model = keras.Model(inputs=inputs, outputs=[target_index,value_out])
        self.register_variables(self.base_model.variables)
    
    def forward(self, input_dict, state, seq_lens):
        model_out, self._value_out = self.base_model(input_dict["obs"])
        return model_out, state

    def value_function(self):
        import tensorflow as tf
        return tf.reshape(self._value_out, [-1])

    def _create_data_reader(self, estimator_action: EstimatorAction, repeat_endlessly: bool = False):
        return PathContextReader(
            vocabs=self.vocabs,
            config=self.config,
            model_input_tensors_former=_KerasModelInputTensorsFormer(estimator_action=estimator_action),
            estimator_action=estimator_action,
            repeat_endlessly=repeat_endlessly)

class _KerasModelInputTensorsFormer(ModelInputTensorsFormer):
    """
    An instance of this class is passed to the reader in order to help the reader to construct the input
        in the form that the model expects to receive it.
    This class also enables conveniently & clearly access input parts by their field names.
        eg: 'tensors.path_indices' instead if 'tensors[1]'.
    This allows the input tensors to be passed as pure tuples along the computation graph, while the
        python functions that construct the graph can easily (and clearly) access tensors.
    """

    def __init__(self, estimator_action: EstimatorAction):
        self.estimator_action = estimator_action

    def to_model_input_form(self, input_tensors: ReaderInputTensors):
        inputs = (input_tensors.path_source_token_indices, input_tensors.path_indices,
                  input_tensors.path_target_token_indices, input_tensors.context_valid_mask)
        if self.estimator_action.is_train:
            targets = input_tensors.target_index
        else:
            targets = {'target_index': input_tensors.target_index, 'target_string': input_tensors.target_string}
        if self.estimator_action.is_predict:
            inputs += (input_tensors.path_source_token_strings, input_tensors.path_strings,
                       input_tensors.path_target_token_strings)
        return inputs, targets

    def from_model_input_form(self, input_row) -> ReaderInputTensors:
        inputs, targets = input_row
        return ReaderInputTensors(
            path_source_token_indices=inputs[0],
            path_indices=inputs[1],
            path_target_token_indices=inputs[2],
            context_valid_mask=inputs[3],
            target_index=targets if self.estimator_action.is_train else targets['target_index'],
            target_string=targets['target_string'] if not self.estimator_action.is_train else None,
            path_source_token_strings=inputs[4] if self.estimator_action.is_predict else None,
            path_strings=inputs[5] if self.estimator_action.is_predict else None,
            path_target_token_strings=inputs[6] if self.estimator_action.is_predict else None
        )
