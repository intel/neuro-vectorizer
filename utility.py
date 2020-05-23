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
import os
import re
import pickle
import subprocess
from extractor_c import CExtractor
import logging
import copy

logger = logging.getLogger(__name__)

#the maximum number of leafs in the LLVM abstract sytnax tree
MAX_LEAF_NODES = os.environ['MAX_LEAF_NODES']
TEST_SHELL_COMMAND_TIMEOUT = os.environ['TEST_SHELL_COMMAND_TIMEOUT']
# pragma line injected for each loop
pragma_line = '#pragma clang loop vectorize_width({0}) interleave_count({1})\n'

def init_runtimes_dict(files,num_loops,VF_len,IF_len):
    '''Used to initialize runtimes dict that stores 
    runtimes for all the files and loops for 
    different VF/IF during training to save time.'''
    runtimes = {}
    one_program_runtimes = [[None]*IF_len for vf in range(VF_len)]
    for f in files:
        runtimes[f] = {}
        for l in range(num_loops[f]):
            runtimes[f][l] = copy.deepcopy(one_program_runtimes)
    return runtimes

def get_bruteforce_runtimes(rundir,files,vec_action_meaning,
                            interleave_action_meaning):
    ''' get all runtimes with bruteforce seach and -O3 
    assuming a single loop per file!'''
    opt_runtimes = {}
    opt_factors = {}
    all_program_runtimes = {}
    one_program_runtimes = [[0]*len(interleave_action_meaning) 
                            for vf in range(len(vec_action_meaning))]
    full_path_header = os.path.join(rundir,'header.c')
    for filename in files:
        opt_runtime = 1e+9
        opt_factor = (1,1)
        for i,VF in enumerate(vec_action_meaning):
            for j,IF in enumerate(interleave_action_meaning):
                rm_cmd = 'rm ' + filename[:-1]+'o '
                if os.path.exists(filename[:-1]+'o'):
                    os.system(rm_cmd)
                cmd1 = 'timeout 4s ' + os.environ['CLANG_BIN_PATH'] + ' -O3 -lm '+full_path_header
                +' ' +filename+' -Rpass=loop-vectorize -mllvm -force-vector-width='
                +str(VF)+' -mllvm -force-vector-interleave='+str(IF)
                +' -o ' +filename[:-1]+'o'
                os.system(cmd1)
                cmd2 = filename[:-1]+'o '
                try:
                    runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True,
                            stdout=subprocess.PIPE).stdout.read())
                except:
                    runtime = None #None if fails
                    logger.warning('Could not compile ' + filename + 
                                   ' due to time out. Setting runtime to: '+str(runtime)+'.' +
                                   ' Consider increasing the timeout, which is set to 4 seconds.')
                one_program_runtimes[i][j] = runtime
                if runtime<opt_runtime:
                    opt_runtime = runtime
                    opt_factor = (VF,IF)
        opt_runtimes[filename] = opt_runtime
        opt_factors[filename] = opt_factor
        all_program_runtimes[filename]=copy.deepcopy(one_program_runtimes)
    data={'opt_runtimes':opt_runtimes,'opt_factors':opt_factors,'all_program_runtimes':all_program_runtimes}
    output = open(os.path.join(rundir,'bruteforce_runtimes.pkl'), 'wb')
    pickle.dump(data, output)
    output.close()

def rename_contents(rundir, contents):
    '''Takes in a run directory, and the contents of the pkl file, renames the directory of the contents
    of the pkl file based on the new rundir specified. It is useful when the user reuses the provided pkl
    file with new rundir.'''
    new_contents = {} 
    for key in contents.keys():
        value = contents[key] 
        suffix_filename = key.split('/')[-1]  # extracts the file name 
        new_path = os.path.join(rundir, suffix_filename)
        new_contents[new_path] = value
    return new_contents 

def get_O3_runtimes(rundir,files):
    '''get all runetimes for O3 (baseline).'''
    try:
        print('Checking if local O3_runtimes.pkl file exists to avoid waste of compilation.') 
        with open(os.path.join(rundir,'O3_runtimes.pkl'), 'rb') as f:
            return rename_contents(rundir, pickle.load(f))
    except:
        print('Did not find O3_runtimes.pkl...', 'Compiling to get -O3 runtimes.')
        pass
    O3_runtimes={}
    full_path_header = os.path.join(rundir,'header.c')
    for filename in files:
        rm_cmd = 'rm ' + filename[:-1]+'o '
        if os.path.exists(filename[:-1]+'o'):
            os.system(rm_cmd)
        cmd1 = 'timeout 2s ' + os.environ['CLANG_BIN_PATH'] +  ' -O3 -lm '+full_path_header +' ' +filename+' -o ' +filename[:-1]+'o'
        print(cmd1)
        os.system(cmd1)
        cmd2 = filename[:-1]+'o '
        try:
            runtime = int(subprocess.Popen(cmd2, executable='/bin/bash', 
                      shell=True, stdout=subprocess.PIPE).stdout.read())
        except:
            runtime = None #None if fails
            logger.warning('Could not compile ' + filename + 
                           ' due to time out. Setting runtime to: ' +
                           str(runtime)+'. Considering increasing the timeout,'+
                           ' which is currently set to 2 seconds.')
        O3_runtimes[filename]=runtime
    output = open(os.path.join(rundir,'O3_runtimes.pkl'), 'wb')
    pickle.dump(O3_runtimes, output)
    output.close()
    return O3_runtimes

def get_snapshot_from_code(code,loop_idx=None):
    ''' take snapshot of the loop code and encapsulate
     in a function declaration so the parser can output
     AST tree.'''
    new_code =[]
    if loop_idx:
        new_code.append('__attribute__((noinline))\n')
        new_code.append('void example() {\n')
        new_code.extend(code[loop_idx[0]:loop_idx[1]+1])
        new_code.extend('}\n')
        return new_code
    found = False
    for line in code:
        if '__attribute__' in line:
            found = True
        if 'int main(' in line:
            break
        if found:
            new_code.append(line)
    return new_code

def get_encodings_from_local(rundir):
    '''returns encodings from obs_encodings.pkl if 
    file exists in trainig directory.'''
    encodings = {}
    print('Checking if local obs_encodings.pkl file exists.') 
    if os.path.exists(os.path.join(rundir,'obs_encodings.pkl')):
        print('found local obs_encodings.pkl.')
        with open(os.path.join(rundir,'obs_encodings.pkl'), 'rb') as f:
            return rename_contents(rundir, pickle.load(f))
    return encodings

def run_llvm_test_shell_command(rundir,filename):
    '''runs the file after the pragma is injected 
    and returns runtime.'''
    full_path_header = os.path.join(rundir, 'header.c')
    cmd1 = 'timeout ' + TEST_SHELL_COMMAND_TIMEOUT + ' ' + os.environ['CLANG_BIN_PATH'] + ' -O3 -lm '+full_path_header \
    +' ' +filename+' -o ' +filename[:-1]+'o'
    cmd2 = filename[:-1]+'o '
    os.system(cmd1)
    try:
        runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
    except:
        runtime = None #None if fails
        logger.warning('Could not compile ' + filename +  
                       ' due to time out. Setting runtime to: ' + 
                       str(runtime)+'. Considering increasing the TEST_SHELL_COMMAND_TIMEOUT,'+ 
                       ' which is currently set to ' + TEST_SHELL_COMMAND_TIMEOUT)
    return runtime

def get_runtime(rundir,new_code,current_filename):
    '''produces the new file with the pragma and 
    compiles to get runtime.'''
    runtime=run_llvm_test_shell_command(rundir,current_filename)
    return runtime


def get_block(i,code):
    j = i
    cnt = 0
    while(True):
        line = code[j]
        if re.match(r'^\s*//',line) or re.match(r'^\s*$',line):
            j += 1
            continue
        if '{' in line:
            cnt += line.count('{')
        if '}' in line:
            cnt -= line.count('}')
        if cnt == 0 and not (re.match(r'^\s*for\s*\(',line) or re.match(r'^\s*while\s*\(',line)):
            return (i,j)
        if cnt == 0 and line.endswith(';\n'):
            return (i,j)
        if (re.match(r'^\s*for\s*\(',line) or re.match(r'^\s*while\s*\(',line)) and i != j:
            return get_block(j,code)
        j=j+1

def get_vectorized_code(code):
    '''Used by get_vectorized_codes function to do the parsing 
    of a single code to detect the loops, inject commented pragmas,
    and collect data.''' 
    new_code = []
    for_loops_indices = []
    i=0
    pragma_indices = []
    num_elems_in_new_code=0
    while i < len(code):
        line=code[i]
        if re.match(r'^\s*for\s*\(',line) or re.match(r'^\s*while\s*\(',line):
            begining,ending = get_block(i,code)
            orig_i=i
            while(i<ending+1):
                if i==begining:
                    new_code.append('//'+pragma_line.format(64,16))#start with -O3 vectorization
                    num_elems_in_new_code += 1
                    pragma_indices.append(num_elems_in_new_code-1)
                new_code.append(code[i])
                num_elems_in_new_code += 1
                i = i+1
            # to pick the index of the most innner loop    
            #for_loops_indices.append((orig_i,ending))
            for_loops_indices.append((begining,ending))
            i=ending+1
            continue
        new_code.append(line)
        num_elems_in_new_code += 1
        i += 1

    return for_loops_indices,pragma_indices,new_code

def get_vectorized_codes(orig_trainfiles, new_trainfiles):
    '''parses the original training files to detect loops.
    Then copies the files to the new directory with
    commented pragmas.'''
    loops_idxs_in_orig = {}
    pragmas_idxs = {}
    const_new_codes ={}
    num_loops = {}
    const_orig_codes={}
    for o_fn,n_fn in zip(orig_trainfiles,new_trainfiles):
        f = open(o_fn,'r')
        try:
            code = f.readlines()
        except:
            f.close()
            continue

        loops_idx, pragmas_idx, new_code = get_vectorized_code(code)
        if not pragmas_idx:
            f.close()
            continue
        const_orig_codes[n_fn] = list(code)
        loops_idxs_in_orig[n_fn]=list(loops_idx)
        pragmas_idxs[n_fn] = list(pragmas_idx)
        const_new_codes[n_fn] = list(new_code)
        num_loops[n_fn] = len(pragmas_idx)
        logger.info('writing file... ' + n_fn)
        nf = open(n_fn,'w')
        nf.write(''.join(new_code))
        nf.close()
        f.close()
    return loops_idxs_in_orig, pragmas_idxs, const_new_codes,num_loops,const_orig_codes
