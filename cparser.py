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
#import clang.cindex
from clang.cindex import Config, Index, CursorKind, TypeKind, LinkageKind, TranslationUnit, StorageClass
from astnode import ASTNode
import argparse
import sys, traceback, os, re
import itertools
import hashlib
import subprocess
import random
import glob

# Regexp to | seperate function names Works on _ and camel case functions (taken from code2vec java)
EXP = re.compile(r"(?<=[a-z])(?=[A-Z])|_|[0-9]|(?<=[A-Z])(?=[A-Z][a-z])|\\s+")
# Find sequences of more than one pipe
DUP = re.compile(r"\|\|+")

def main():
    parser = argparse.ArgumentParser("Parse c code to path objects")
    parser.add_argument("--clang-path", dest="clang_path",
                      help="Path to clang shared library", metavar="CLANG", type=str, required=True)
    parser.add_argument("--file-path", dest="file_path", default=None, type=str, 
                      help="Path to a single file to parse", metavar="FILE")
    parser.add_argument("--dir-path", dest="dir_path", default=None,
                      help="Parse all *.c files in this directory and children.", metavar="DIR", type=str)
    parser.add_argument("--include-path", dest="include_path",
                       help="Hardcode this path into includes", metavar="INC", type=str, default=None)
    parser.add_argument("--max-leaves", dest="max_leaves",
                      help="(int) Do not process functions with more than L leaves",
                      metavar="L", type=int, default=None)
    parser.add_argument("--no-hash-paths", dest="hash_paths",
                      help="(bool) If defined will not hash code paths",
                      metavar="HASH", type=bool, default=False)
    parser.add_argument("--dump-tree", dest="dump_tree",
                      help="(bool) Print a tree representation to stderr",
                      metavar="PRINT", type=bool, default=False)
    parser.add_argument("--dump-nodes", dest="dump_nodes",
                      help="(bool) Dump unique nodes to stderr",
                      metavar="NODES", type=bool, default=False)
    parser.add_argument("--skip-decls", dest="skip_decls",
                      help="(bool) skip function declarations instead of tagging them",
                      metavar="DECLS", type=bool, default=False)                      

    set_args(parser.parse_args())
    if ARGS.dir_path is not None: os.chdir(ARGS.dir_path)
    
    configure_clang(ARGS.clang_path)
    index = Index.create()
    
    include_dirs = setup_includes()
    
    dir_path = ARGS.dir_path
    if dir_path is not None:
        dir_path = os.path.normpath(dir_path)
        include_path = append_include(dir_path)
        add_dir_if_exists(include_dirs, include_path)

        for dir in [x[0] for x in os.walk(dir_path)]:
            inner_dirs = [dir]
            include_path = append_include(dir)
            add_dir_if_exists(inner_dirs, include_path)
            files = glob.glob(dir.rstrip(os.sep) + os.sep + "*.c")
            for file in files:
                try:
                    parse_single(file, include_dirs + inner_dirs, index)    
                except:
                    sys.stderr.write("Exception parsing file:"+file+"\n")
                    traceback.print_tb(sys.exc_info()[2])
    elif ARGS.file_path is not None:
        parse_single(os.path.normpath(ARGS.file_path), include_dirs, index)
    else: sys.stderr.write("Invoked without dir_path or file_path")

def setup_includes():
    dir = []
    include_path = ARGS.include_path
    if include_path is not None: add_dir_if_exists(dir, include_path)
    return dir

def configure_clang(clang_path):
    Config.set_library_file(clang_path)

def set_args(_args):
    global ARGS 
    ARGS = _args

def add_dir_if_exists(list, path):
    norm_path = os.path.normpath(path)
    if os.path.isdir(norm_path):
        list.append(norm_path)

def append_include(path):
    return os.path.join(path, "include")

# Parse a single file print to standard output
def parse_single(file_path, include_dirs, index):
    # Generate random filename for in-memory file
    destname = file_path + format(random.randint(0,99999), '05d') + ".c"
    cmd = ["clang"]
    if include_dirs is not None:
        cmd.extend(["-I"+i for i in include_dirs])
    cmd.extend(["-E", file_path])

    try: 
        pre_processed_file = subprocess.check_output(cmd)
    except subprocess.CalledProcessError:
        # We will get error code 2 if we can't find headers...its normal
        ex_type, ex, tb = sys.exc_info()
        pre_processed_file = ex.output

    if len(pre_processed_file) <= 0:
        raise Exception("Parsed file is zero length: "+file_path)
    tu = index.parse(destname, unsaved_files=[(destname, pre_processed_file)])
    root_level(tu.cursor.get_children())

# Prints tree to stderr, or f if provided
def traverse_to_print(node, f=sys.stderr):
    f.write(node.ToOutputString()+"\n")
    if node.children != None:
        map(lambda n: traverse_to_print(n, f), node.children)

def visit(current, parent, depth):
    this = ASTNode(parent, current, depth)
    return this

# We traverse a tree to create a duplicate tree that contains ASTNode
# objects.  These objects provide quick data acess and cahcing of values
# in the clang tree.
# This function creates nodes in pre-order, sets children post-order.
# Doing operations in this order is necessary given how ASTNode operates.
def traverse(current_clang, parent=None, depth=0, expansion=False):
    node = visit(current_clang, parent, depth)
    children = [traverse(c, node, depth+1, expansion) for c in current_clang.get_children()]
    node.SetChildren(children)
    return node

def root_level(top_nodes):
    functions = []
    for top_node in top_nodes:
        # Only care about nodes in this file
        if top_node.location.file is None \
            or top_node.location.file.name != top_node.translation_unit.spelling \
            or top_node.storage_class == StorageClass.EXTERN:
            continue

        if top_node.kind == CursorKind.FUNCTION_DECL:
            try:
                func_root = traverse(top_node)
                functions.append(func_root)
                if ARGS.dump_tree: traverse_to_print(func_root)
                generate_and_print_paths(func_root)
            except Exception:    
                # Print and eat all excptions so we don't stop parsing for one bad function.
                sys.stderr.write("Exception parsing function:"+top_node.location.__str__()+"\n")
                traceback.print_tb(sys.exc_info()[2])
    if ARGS.dump_nodes:
        sys.stderr.write("Internal Node Types:\n\t"+str(functions[0].children[0].unique_cursors)+"\n")
        sys.stderr.write("Leaf Node Types:\n\t"+str(functions[0].children[0].unique_leaves)+"\n")
    return functions

def get_all_leaves(node, result):
    if node.leaf:
        return result.append(node)
    for l in node.children: get_all_leaves(l, result)

# Walk from a node to the root...collecting nodes on the way
def walk_to_root(node):
    result = []
    n = node
    while n.parent != None:
        result.append(n)
        n = n.parent
    result.append(n)
    return result


# Given two lists from an item to the root find the first common ancestor
# Returns tuple of the path and the location of the root node.
def find_common_path(l1, l2):
    for x in range(1, min(len(l1), len(l2))+1):
        if(l1[-x] is not l2[-x]):
            # Error case
            if x < 2: raise Exception("First common ancestor is first node")
            # Get all nodes but the last x from both lists but include the common ancestor in first list
            if x-2 <= 0:
                l1_slice = slice(None, None)
            else:
                l1_slice = slice(None, -(x-2))
            l2_slice = slice(None, -(x-1))
            return l1[l1_slice] + list(reversed(l2[l2_slice])), len(l1)-x+1

# Pipe seperate function name, more than one | in a row is reduced to |, strip leading and trailing |s
def normalize_function_name(str):
    return DUP.sub("|", "|".join(EXP.split(str))).strip("|")


# Iterate from every leaf to every other leaf printing the path.
# Doesn't store paths for memory efficency on large functions.
def generate_and_print_paths(function, f=sys.stdout):
    decl_tag = "functiondecl_"
    func_name = function.displayname
    leaves = []
    get_all_leaves(function, leaves)
    if len(leaves) <= 1: return # Won't have any code paths
    # These lines are a departute from how code2vec parses trees
    # the behavior here can be modifed to use a filtering mechanism other
    # than total leaf nodes.
    if ARGS.max_leaves is not None and len(leaves) > ARGS.max_leaves:
        sys.stderr.write("Skipping Function: "+ function.displayname + ", becuase leaf count of: "+str(len(leaves))+"\n")
        return
    
    # Java does not have decls but we need to deal with them 
    # in C, should we skip or tag them?
    if not function.IsFunctionDefinition():
        if ARGS.skip_decls:
            sys.stderr.write("Skipping Function: "+ function.displayname + ", becuase it is a decl." + "\n")
            return
        else:
            func_name = decl_tag + func_name
    f.write(normalize_function_name(func_name))
    for s in leaves:
        uptree = walk_to_root(s)
        for e in leaves:
            if e is s: continue
            downtree = walk_to_root(e)
            path, pivot = find_common_path(uptree, downtree)
            uptree_cp = map(lambda o: o.ToCodePathNode(), path[:pivot])
            downtree_cp = map(lambda o: o.ToCodePathNode(), path[pivot:])
            f.write(" " +",".join([s.value,generate_pathstring(uptree_cp, downtree_cp, not ARGS.hash_paths), e.value]))
    f.write("\n")
    f.flush()

# Create a pathstring from two node lists
def generate_pathstring(uptree, downtree, hash_path=False):
    s = "^".join(uptree) +"^"+ "_".join(downtree)
    if hash_path:
        return hashlib.sha256(s.encode()).hexdigest()
    return s

if __name__ == '__main__':
    # Wrap in a try, we don't want exceptions escaping
    try:
        main()
    except:
        sys.stderr.write("Exception parsing dirctory\n")
        traceback.print_tb(sys.exc_info()[2])
