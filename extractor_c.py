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
import subprocess
import hashlib

# Generate a basic cmd array
def generate_cmd(args):
    command = ['python', './preprocess/cparser.py', "--clang-path", args.clang_path, "--max-leaves", args.max_leaves, "--no-hash-paths", "TRUE"]
    if args.include_path is not None and len(args.include_path) > 0:
        command.extend(["--include-path", args.include_path])
    return command

class TmpObject(object):
    pass

class CExtractor:
    def __init__(self, config, clang_path, max_leaves):
        self.args = TmpObject()
        self.args.config = config
        self.args.max_leaves = str(max_leaves)
        #self.max_path_width = max_path_width
        self.args.clang_path = clang_path
        self.args.include_path = None

    def extract_paths(self, path):
        #command = ['java', '-cp', self.jar_path, 'JavaExtractor.App', '--max_path_length',
        #           str(self.max_path_length), '--max_path_width', str(self.max_path_width), '--file', path, '--no_hash']
        command = generate_cmd(self.args)
        command.extend(["--file-path",path])
        print("Executing: "+" ".join(command))
        process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = process.communicate()
        output = out.decode().splitlines()
        if len(output) == 0:
            err = err.decode()
            raise ValueError(err)
        hash_to_string_dict = {}
        result = []
        for i, line in enumerate(output):
            parts = line.rstrip().split(' ')
            method_name = parts[0]
            current_result_line_parts = [method_name]
            contexts = parts[1:]
            for context in contexts[:self.args.config.MAX_CONTEXTS]:
                context_parts = context.split(',')
                context_word1 = context_parts[0]
                context_path = context_parts[1]
                context_word2 = context_parts[2]
                hashed_path = str(self.c_string_hashcode(context_path))
                hash_to_string_dict[hashed_path] = context_path
                current_result_line_parts += ['%s,%s,%s' % (context_word1, hashed_path, context_word2)]
            space_padding = ' ' * (self.args.config.MAX_CONTEXTS - len(contexts))
            result_line = ' '.join(current_result_line_parts) + space_padding
            result.append(result_line)
        return result, hash_to_string_dict

    @staticmethod
    def c_string_hashcode(s):
        """
        Do the same hashing we do in cparser.py
        """
        return hashlib.sha256(s.encode()).hexdigest()
