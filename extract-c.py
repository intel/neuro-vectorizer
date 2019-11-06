# coding=utf-8
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
#!/usr/bin/python

import itertools
import multiprocessing, threading
import os
import glob
import sys
import shutil
import subprocess
import gzip, io
from threading import Timer
import sys
from pathlib import Path
from argparse import ArgumentParser
from subprocess import Popen, PIPE, STDOUT, call



def get_immediate_subdirectories(a_dir):
    return [(os.path.join(a_dir, name)) for name in os.listdir(a_dir)
            if os.path.isdir(os.path.join(a_dir, name))]


TMP_DIR = ""

def ParallelExtractDir(args, dir):
    ExtractFeaturesForDir(args, dir, "")


def background_function(file, stdout):
    shutil.copyfileobj(stdout, file)

# Generate a basic cmd array
def generate_cmd(args):
    command = ['python', 'cparser.py', "--clang-path", args.clang_path, "--max-leaves", args.max_leaves]
    if args.include_path is not None and len(args.include_path) > 0:
        command.extend(["--include-path", args.include_path])
    if args.decls is not None:
        command.extend(["--skip-decls", args.decls])
    return command

def ExtractFeaturesForDir(args, dir, prefix):
    sys.stderr.write('visiting dir: ' + str(dir)+"\n")
    outputFileName = TMP_DIR + prefix + dir.split('/')[-1] + ".gz"
    command = generate_cmd(args)
    command.extend(["--dir-path", dir, "| gzip -9 >> ", outputFileName])
    kill = lambda process: process.kill()
    failed = False
    # Gotta shell this becuase of IORedirection doesn't work well in python so this 
    # is the only way to get output gziped
    sleeper = subprocess.Popen(" ".join(command), shell=True, stdout=subprocess.PIPE)
    timer = Timer(600000, kill, [sleeper])
    try:
        timer.start()
        stdout, stderr = sleeper.communicate()
        #relay stderr...
    finally:
        timer.cancel()
    if sleeper.poll() == 0:
        if stderr is not None and len(stderr) > 0:
            sys.stderr.write(stderr)
    else:
        sys.stderr.write( 'dir: ' + str(dir) + ' was not completed in time\n')
        failed = True
        subdirs = get_immediate_subdirectories(dir)
        for subdir in subdirs:
            ExtractFeaturesForDir(args, subdir, prefix + dir.split('/')[-1] + '_')
    if failed:
        if os.path.exists(outputFileName):
            os.remove(outputFileName)


def ExtractFeaturesForDirsList(args, dirs):
    global TMP_DIR
    TMP_DIR = args.outdir + "/feature_extractor/"
    if os.path.exists(TMP_DIR):
        shutil.rmtree(TMP_DIR, ignore_errors=True)
    os.makedirs(TMP_DIR)
    try:
        p = multiprocessing.Pool(int(args.num_threads))
        p.starmap(ParallelExtractDir, zip(itertools.repeat(args), dirs))
        #output_files = os.listdir(TMP_DIR)
        #for f in output_files:
        os.system("zcat %s/%s" % (TMP_DIR, "*"))
    finally:
        sys.stderr.write('Dumped files, saving intermediates.')
        shutil.rmtree(TMP_DIR, ignore_errors=True)


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument("-clang-path", "--clang-path", dest="clang_path", required=True)
    parser.add_argument("--include-path", dest="include_path", default=None)
    parser.add_argument("-maxleaves", "--max-leaves", dest="max_leaves", required=False, default=1000)
    parser.add_argument("-threads", "--num_threads", dest="num_threads", required=False, default=8)
    parser.add_argument("-dir", "--dir", dest="dir", required=False)
    parser.add_argument("-outdir", "--outdir", dest="outdir", required=True)
    parser.add_argument("-file", "--file", dest="file", required=False)
    parser.add_argument("-skipdecls", "--skip-decls", dest="decls", required=False)
    args = parser.parse_args()

    if args.file is not None:
        command = generate_cmd(args)
        command.extend(["--file-path", args.file])
        #command = ['python', 'cparser.py', "--clangpath" arg.clang_path, "--file-path", arg.file]
        os.system(command)
    elif args.dir is not None:
        subdirs = get_immediate_subdirectories(args.dir)
        to_extract = subdirs
        if len(subdirs) == 0:
            to_extract = [args.dir.rstrip('/')]
        ExtractFeaturesForDirsList(args, to_extract)


