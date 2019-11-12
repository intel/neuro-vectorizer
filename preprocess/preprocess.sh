#!/usr/bin/env bash
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

# Configuration is relative to this script
SCRIPT_LOC=$(realpath "$0")
SCRIPT_PATH="$(dirname "$SCRIPT_LOC")"
. $SCRIPT_PATH/configure.sh

[ ! -d ${CODE2VEC_LOC} ] && echo "code2vec location does not exist!" && exit 1
[ ! -d ${SOURCE_DIR} ] && echo "source directory does not exist!" && exit 1

DATA_ROOT=${CODE2VEC_LOC}/data/${DATASET_NAME}
RAW_DATA_FILE=${DATA_ROOT}/${DATASET_NAME}.raw.txt
SORTED_DATA_FILE=${DATA_ROOT}/${DATASET_NAME}.sorted.raw.txt
TRAIN_DATA_FILE=${DATA_ROOT}/${DATASET_NAME}.train.raw.txt
VAL_DATA_FILE=${DATA_ROOT}/${DATASET_NAME}.val.raw.txt
TEST_DATA_FILE=${DATA_ROOT}/${DATASET_NAME}.test.raw.txt

# Ensure previous runs are cleared out...
rm -rf $DATA_ROOT
mkdir -p $DATA_ROOT

echo "Extracting paths from data set..."
${PYTHON} ${SCRIPT_PATH}/extract-c.py --dir ${SOURCE_DIR} -outdir ${DATA_ROOT} ${SKIP_DECLS} -maxleaves ${MAX_LEAVES} --num_threads ${NUM_THREADS} --clang-path ${CLANG_PATH} --include-path ${SOURCE_DIR} > ${RAW_DATA_FILE} 2>> ${DATA_ROOT}/stderr.txt
echo "Finished extracting paths from data set"
sort -u -R -S ${MEM_PERCENT}% --parallel ${NUM_PROCESSORS} --output=${SORTED_DATA_FILE} ${RAW_DATA_FILE}

echo "Calculating lines"
n=$(wc -l < ${SORTED_DATA_FILE})
[[ ! "$n" -gt "0" ]] && echo "Extracted no lines. (missing source files?)" && exit 1

n=$(echo "x=${n} * ${DOWNSAMPLE}; scale = 0; x / 1" | bc -l)
train_n=$(echo "x=${n} * 0.99; scale = 0; x / 1" | bc -l)
test_n=$(echo "x=(${n} - ${train_n}) / 2; scale = 0; x / 1" | bc -l)
# val_n is the rest
train=$train_n
test=$test_n
val="$(( $n - ( $train + $test )))"

echo "N: $n, train_n ${train}, test_n ${test}, val_n ${val}"
echo "Extracting samples to raw files."
cat ${SORTED_DATA_FILE} | { head -n ${train} > ${TRAIN_DATA_FILE}; head -n ${test} > ${TEST_DATA_FILE}; head -n ${val} > ${VAL_DATA_FILE}; }
echo "Done extracting samples to raw files."

TARGET_HISTOGRAM_FILE=${DATA_ROOT}/${DATASET_NAME}.histo.tgt.c2v
ORIGIN_HISTOGRAM_FILE=${DATA_ROOT}/${DATASET_NAME}.histo.ori.c2v
PATH_HISTOGRAM_FILE=${DATA_ROOT}/${DATASET_NAME}.histo.path.c2v

echo "Creating histograms from the training data"
cat ${TRAIN_DATA_FILE} |  tee \
>(cut -d' ' -f1 | awk '{n[$0]++} END {for (i in n) print i,n[i]}' > ${TARGET_HISTOGRAM_FILE}) \
| cut -d' ' -f2- | tr ' ' '\n' | tee \
>(cut -d',' -f1,3 | tr ',' '\n' | awk '{n[$0]++} END {for (i in n) print i,n[i]}' > ${ORIGIN_HISTOGRAM_FILE}) \
| cut -d',' -f2 | sort -S ${MEM_PERCENT}% --parallel ${NUM_PROCESSORS} | uniq -c | awk '{ t = $1; $1 = $2; $2 = t; print; }' > ${PATH_HISTOGRAM_FILE}
echo "Done creating histograms...sending to code2vec for preprocessing"

${PYTHON} ${CODE2VEC_LOC}/preprocess.py --train_data ${TRAIN_DATA_FILE} --test_data ${TEST_DATA_FILE} --val_data ${VAL_DATA_FILE} \
  --max_contexts ${MAX_CONTEXTS} --word_vocab_size ${WORD_VOCAB_SIZE} --path_vocab_size ${PATH_VOCAB_SIZE} \
  --target_vocab_size ${TARGET_VOCAB_SIZE} --word_histogram ${ORIGIN_HISTOGRAM_FILE} \
  --path_histogram ${PATH_HISTOGRAM_FILE} --target_histogram ${TARGET_HISTOGRAM_FILE} --output_name ${DATA_ROOT}/${DATASET_NAME}
    
# This line can be uncommented if disk usage is of primary concern.  This will delete intermediary files which are 
# redundant and sizeable.
#rm ${TRAIN_DATA_FILE} ${VAL_DATA_FILE} ${TEST_DATA_FILE} ${TARGET_HISTOGRAM_FILE} ${ORIGIN_HISTOGRAM_FILE} \
#  ${PATH_HISTOGRAM_FILE} ${RAW_DATA_FILE} ${SORTED_DATA_FILE}

