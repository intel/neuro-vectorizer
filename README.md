<a href="https://scan.coverity.com/projects/intel-academic-neurovectorizer">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/20079/badge.svg"/>
</a>

## NeuroVectorizer v0.0.1
Neurovectorizer is a framework that uses deep reinforcement learning (RL) to predict optimal vectorization compiler pragmas for `for loops` in C and C++ codes. The framework currently integrates with the LLVM compiler and can inject vectorization and interleaving factors. It is possible to support unrolling factors too by adding it as an action in the RL environment. More details are available in the paper. This paper appeared in CGO2020 and Passed all the artifact evaluations for reproducability.

Dependencies:
- TF2 (`pip install tensorflow`).
- Ray (`pip install ray==0.8.4`).
- RLlib (`pip install ray[rllib]==0.8.4`).
- LLVM (you need to have `/usr/lib/llvm-X.Y/lib/libclang.so.1` or equivalent working). Currently tested with `/usr/lib/llvm-6.0/lib/libclang.so.1`.
- clang (`pip install clang`). Current tested version is clang-6.0.0.2.  

For more detailed install instructions tested on Ubuntu click [here.](detailedinstructions.md)

The framework takes the text code of loops (detects them in the code) and uses an AST embedding generator. The output of this generator is fed to a neural network agent that predicts the optimal factors.

# To run training run:
```
- cd preprocess
- source ./configure.sh //you might need here to modify SOURCE_DIR to point to your train data.
- source ./preprocess.sh //this will generate the bag of words embedding of the AST trees for the training data (the training set is in "training_data" feel free to add more samples).
- python autovec.py
```
**Important notes:**
- Some of the error messages when running `source ./preprocess.sh` are dumped to `code2vec/data/for_loops/stderr.txt`.
- Training might take a long time to finish.
- autovec.py uses the RLLib/TUNE API explained here: https://ray.readthedocs.io/en/latest/tune-package-ref.html.
- O3_runtimes.pkl and obs_encodings.pkl are provided in `./training_data`. O3_runtimes.pkl stores the -O3 runtimes on Intel® AVX Intel® Xeon® Processor E5-2667 v2 and obs_encodings.pkl stores the encodings of the AST programs so that you don't have to recompute it when training on the training data. **If you have another Processor, remove O3_runtimes.pkl or else it will use -O3 runtimes based on the wrong processor**!
- If you want to use another model in the embedding generator, you need to modify `get_obs` function in "envs/neurovec.py".

# To run rollout/inference on files in the provided dataset\*:
`python temp_rollout.py <~/ray_results/NeuroVectorizer/PPO_NeuroVectorizerEnv_*/checkpoint_*/checkpoint-*> --rollout_dir \
<./rollout_data> --compile`

\* If it is not in the dataset then use the --new_train_data flag.

Note that this command will raise `ray.exceptions.RayActorError: The actor died unexpectedly before finishing this task`. This error is due to killing the ray worker after inferencing all the files. **Ignore this error**.

# The provided pretrained model:
A very basic pretrained model is provided as three checkpoints under `./checkpoints` that you can use to navigate and exercise. 
For example:
you can run `python temp_rollout.py checkpoints/checkpoint_100/checkpoint-100 --rollout_dir "./tests" --compile`



Please reach out to Ameer Haj Ali for any questions.


To cite this work:
```
@inproceedings{ameerhajalicgo,
 author = {Haj-Ali, Ameer and Ahmed, Nesreen and Willke, Ted and Shao, Sophia and Asanovic, Krste and Stoica, Ion},
 title = {NeuroVectorizer: End-to-End Vectorization with Deep Reinforcement Learning},
 booktitle = {Proceedings of the 2020 International Symposium on Code Generation and Optimization},
 series = {CGO 2020},
 year = {2020},
 location = {San Diego, USA},
 publisher = {ACM},
} 
```
