import os
import re
import pickle
import subprocess
from extractor_c import CExtractor

MAX_LEAF_NODES = 320
pragma_line = '#pragma clang loop vectorize_width({0}) interleave_count({1})\n'

#Used to get runtimes of brute force search, O3, and get code embeddings (currently implements code2vec code embedding)
# get all runtimes with bruteforce seach
def get_bruteforce_runtimes(rundir,files,vec_actions,interleave_actions):
    opt_runtimes = {}
    opt_factors = {}
    all_program_runtimes = {}
    one_program_runtimes = [[0]*len(interleave_action_meaning) for vf in range(len(vec_action_meaning))]
    full_path_header = os.path.join(rundir,'header.c')
    for filename in files:
        opt_runtime = 1e+9
        opt_factor = (1,1)
        for i,VF in enumerate(vec_action_meaning):
            for j,IF in enumerate(interleave_action_meaning):
                rm_cmd = 'rm ' + filename[:-1]+'o '
                os.system(rm_cmd)
                cmd1 = 'timeout 4s /usr/bin/clang -O3 '+full_path_header +' ' +filename+' -Rpass=loop-vectorize -mllvm -force-vector-width='+str(VF)+' -mllvm -force-vector-interleave='+str(IF)+' -o ' +filename[:-1]+'o'# TODO: fix path
                os.system(cmd1)
                cmd2 = filename[:-1]+'o '
                try:
                    runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
                except:
                    runtime = 1000000 #inf if fails replace with 5 times O3 runtimes
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


# get all runetimes for O3 (baseline)
def get_O3_runtimes(rundir,files,vec_actions,interleave_actions):
#    try:
#        with open('./O3_runtimes.pkl', 'rb') as f:
#            return pickle.load(f)
#    except:
#        pass
    O3_runtimes={}
    full_path_header = os.path.join(rundir,'header.c')
    for filename in files:
        rm_cmd = 'rm ' + filename[:-1]+'o '
        os.system(rm_cmd)
        cmd1 = 'timeout 2s /usr/bin/clang -O3 '+full_path_header +' ' +filename+' -o ' +filename[:-1]+'o'# TODO: fix path
        print(cmd1)
        os.system(cmd1)
        cmd2 = filename[:-1]+'o '
        try:
            runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
        except:
            runtime = 1000000 #inf if fails replace with 5 times O3 runtimes
        O3_runtimes[filename]=runtime

    output = open(os.path.join(rundir,'O3_runtimes.pkl'), 'wb')
    pickle.dump(O3_runtimes, output)
    output.close()
    return O3_runtimes

# take snapshot of the loop code and encapsulate in a function declaration so the parser can ouput AST tree
def get_snapshot_from_code(code,loop_idx=None):
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

# works with old versions of code2vec
def c_code2vec_get_encodings(rundir,const_orig_codes,loops_idxs_in_orig):
    from code2vec_old.model import Model
    from code2vec_old.common import Config
    encodings={}
    config = Config.get_default_config()
    model = Model(config)
    print('created model')
    path_extractor = CExtractor(config,
                                     clang_path=os.environ['CLANG_PATH'],
                                     max_leaves=MAX_LEAF_NODES)
    input_full_path_filename = os.path.join(rundir, 'c_code2vec_input.c')
    print(input_full_path_filename)
    for key in const_orig_codes.keys():
        encodings[key] = {}
        for idx,loop_idx in enumerate(loops_idxs_in_orig[key]):
            ## take for loop from teh code ##
            code = get_snapshot_from_code(const_orig_codes[key],loop_idx)
            ## endo of work around ##
            loop_file=open(input_full_path_filename,'w')
            loop_file.write(''.join(code))
            loop_file.close()
            predict_lines, hash_to_string_dict = path_extractor.extract_paths(input_full_path_filename)
            print('predict lines:',predict_lines)
            print('hash:',hash_to_string_dict)
            results, code_vectors = model.predict(predict_lines)
            print(sum(code_vectors[0]))
            print(code)
            encodings[key][idx] = code_vectors[0]
    model.close_session()
    print(encodings)
    output = open(os.path.join(rundir,'c_code2vec_encodings.pkl'), 'wb')
    pickle.dump(encodings, output)
    output.close()
    return encodings

''' works with new versions of code2vec
def c_code2vec_get_encodings(rundir,const_orig_codes,loops_idxs_in_orig):
    from rollout_config import Config
    from tensorflow_model import Code2VecModel
    encodings={}
    config = Config(set_defaults=True, load_from_args=False, verify=True)
    model = Code2VecModel(config)
    print('created model')
    path_extractor = CExtractor(config,
                                     clang_path=os.environ['CLANG_PATH'],
                                     max_leaves=MAX_LEAF_NODES)
    input_full_path_filename = os.path.join(rundir, 'c_code2vec_input.c')
    print(input_full_path_filename)
    for key in const_orig_codes.keys():
        encodings[key] = {}
        for idx,loop_idx in enumerate(loops_idxs_in_orig[key]):
            ## take for loop from teh code ##
            code = get_snapshot_from_code(const_orig_codes[key],loop_idx)
            ## endo of work around ##
            loop_file=open(input_full_path_filename,'w')
            loop_file.write(''.join(code))
            loop_file.close()
            predict_lines, hash_to_string_dict = path_extractor.extract_paths(input_full_path_filename)
            print('predict lines:',predict_lines)
            print('hash:',hash_to_string_dict)
            results, code_vectors = model.predict(predict_lines)
            print(sum(code_vectors[0]))
            print(code)
            encodings[key][idx] = code_vectors[0]
    model.close_session()
    print(encodings)
    output = open(os.path.join(rundir,'c_code2vec_encodings.pkl'), 'wb')
    pickle.dump(encodings, output)
    output.close()
    return encodings
'''
# runs the file after the pragma is injected and returns runtime
def run_llvm_test_shell_command(rundir,filename):
    full_path_header = os.path.join(rundir,'header.c')
    cmd1 = '/usr/bin/clang -O3 '+full_path_header+' ' +filename+' -o ' +filename[:-1]+'o'# TODO: fix path
    cmd2 = filename[:-1]+'o '
    os.system(cmd1)
    runtime=int(subprocess.Popen(cmd2, executable='/bin/bash', shell=True, stdout=subprocess.PIPE).stdout.read())
    return runtime

# produces the new file with the pragma and compiles to get runtime
def get_runtime(rundir,new_code,current_filename):
    tf = open(current_filename,'w')
    tf.write(''.join(new_code))
    tf.close()
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

def get_vectorized_codes(testfiles, new_testfiles):
    loops_idxs_in_orig = {}
    pragmas_idxs = {}
    const_new_codes ={}
    num_loops = {}
    const_orig_codes={}
    for o_fn,n_fn in zip(testfiles,new_testfiles):
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
        print('writing file...',n_fn)
        nf = open(n_fn,'w')
        nf.write(''.join(new_code))
        nf.close()
        f.close()
    return loops_idxs_in_orig, pragmas_idxs, const_new_codes,num_loops,const_orig_codes
