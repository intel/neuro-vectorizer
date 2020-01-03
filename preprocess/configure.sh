#!/usr/bin/env bash
# 
# Copyright (c) 2019, Ameer Haj Ali (UC Berkeley), and Intel Corporation
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# C extractor for code2vec
#
# Copyright 2019 Carnegie Mellon University. All Rights Reserved.
#
# NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
# Released under a MIT (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.
# [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.
# Carnegie Mellon® and CERT® are registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.
# This Software includes and/or makes use of the following Third-Party Software subject to its own license:
# 1. code2vec (https://github.com/tech-srl/code2vec/blob/master/LICENSE) Copyright 2018 Technion.
# 2. LLVM / CLANG (https://github.com/llvm-mirror/clang/blob/master/LICENSE.TXT) Copyright 2019 LLVM.
# DM19-0540
###########################################################
# Change the following values to preprocess a new dataset.
# NUM_THREADS - the number of parallel threads to use. It is 
#   recommended to use a multi-core machine for the preprocessing 
#   step and set this value to the number of cores.
# PYTHON - python3 interpreter alias.
###########################################################

# External dependencies
# CLANG_PATH - should be set to the location of lib clang
CLANG_PATH=/usr/lib/llvm-6.0/lib/libclang.so.1
# CODE2VEC_LOC - should be the location of base codevec
CODE2VEC_LOC=../code2vec
# SOURCE_DIR - top level folder for C source files
SOURCE_DIR=../training_data
# NeuroDectorier DIR
export NeuroVectorizer_DIR=../NeuroVectorizer

### Give or use less resources
# MEM_PERCENT - for configurable commands like sort limit increase memory percentage to use
MEM_PERCENT=75
# NUM_PROCESSORS - for configurable commands like sort raise the number of processors to use
NUM_PROCESSORS=2 
# MAX_LEAVES - maximum number of leaves in an AST for parsing 
#    (default 32, remeber function complexity is ~ leaves^2)
MAX_LEAVES=48
# DOWNSAMPLE - Reduce the size of the dataset.  Change to a percentage (ex. DOWNSAMPLE=.8)
# to only use a portion of the available features.
DOWNSAMPLE=1
# SKIP_DECLS - Comment out this line to tag function decls instead of skipping them.
SKIP_DECLS=--skip-decls true

# code2vec parameters
# DATASET_NAME is just a name for the currently extracted dataset.                                              
# MAX_CONTEXTS is the number of contexts to keep for each 
#   method (by default 200).                              
# WORD_VOCAB_SIZE, PATH_VOCAB_SIZE, TARGET_VOCAB_SIZE -   
#   - the number of words, paths and target words to keep 
#   in the vocabulary (the top occurring words and paths will be kept). 
#   The default values are reasonable for a Tesla K80 GPU 
#   and newer (12 GB of board memory).

DATASET_NAME=for_loops
MAX_CONTEXTS=200
WORD_VOCAB_SIZE=1301136
PATH_VOCAB_SIZE=911417
TARGET_VOCAB_SIZE=261245
NUM_THREADS=64
PYTHON=python3

# Don't edit below this line
CODE2VEC_LOC=$(realpath ${CODE2VEC_LOC})
CLANG_PATH=$(realpath ${CLANG_PATH})
SOURCE_DIR=$(realpath ${SOURCE_DIR})
