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
# Simple class to represent an AST Node as we need fields beyond what clang provides.
# Also provides abstractions and quick access methods into the clang AST.
from clang.cindex import CursorKind, TokenKind
import sys, copy, re, itertools

# A wrapper around (clangcursor) to extend functionality, provide pre-computed caching,
# and aid in pre / post traversal operations.
class ASTNode:
    # Class variables
    remove_pattern = re.compile(r"\W+")
    # Lists of unique nodes for debugging.
    unique_cursors = set()
    unique_leaves = set()

    def __init__(self, parent=None, current=None, depth=0):
        self.parent = parent
        self.children_set = False
        self.children = None
        self.nodetype = current.kind
        self.optype =current.type.spelling
        self.value = None
        self.displayname = current.displayname
        self.leaf = False
        self.depth = depth
        self.clangcursor=current
        self.SetValue()

    def GetTokens(self):
        return self.clangcursor.get_tokens()
    
    def GetFirstToken(self):
        return next(self.clangcursor.get_tokens(), None)

    def SetChildren(self, children):
        self.children_set = True
        if len(children) >= 1:
            self.leaf = False 
            self.children = children
        else:
            self.leaf = True
        self.SetValue()

    def SetValue(self):
        # For tracking purposes
        self.unique_cursors.add(self.nodetype)
        if self.leaf: self.unique_leaves.add(self.nodetype)

        # If we have not set children use first not none of spelling, displayname, first token, optype, nodetype
        if not self.children_set:
            self.value = next(v for v in [self.clangcursor.spelling, self.displayname, \
                            self.GetFirstToken(), self.optype, self.nodetype.name] if v is not None)
            return
        
        #Typically we just want the value off the clangcursor so start with that and dispatch for special cases
        self.value = self.clangcursor.spelling
        if self.nodetype == CursorKind.FUNCTION_DECL:
            self.value = self.nodetype.name
            self.displayname = self.clangcursor.spelling
        elif self.leaf:
            self.SetLeafNodeValue()
        else:
            self.SetInternalNodeValue()

    def SetInternalNodeValue(self):
        # If we have an operator we need to figure out what it is
        r_begin = get_start(self.clangcursor)
        r_end = get_end(self.clangcursor)
        if self.leaf == False and (self.nodetype == CursorKind.UNARY_OPERATOR or self.nodetype == CursorKind.CXX_UNARY_EXPR):
            self.value = None
            op1_begin = get_start(self.children[0].clangcursor)
            op1_end = get_end(self.children[0].clangcursor)
            if eq_clang_loc(r_begin, op1_begin):
                s = [op1_end, r_end]
            else:
                s = [r_begin, op1_begin]
            # Our preferrred match
            self.value = find_clang_token_by_start(self.GetTokens(), s[0], 0)
            # Occasionally unary operators have a shifted offset, so check for that...
            if self.value is None: 
                self.value = find_clang_token_by_start(self.GetTokens(), s[0], 1)
            # Sanity exception, should never hit
            if self.value is None: raise Exception("No unary operator found")
        elif self.nodetype == CursorKind.BINARY_OPERATOR \
            or self.nodetype == CursorKind.COMPOUND_ASSIGNMENT_OPERATOR:
            op2_begin = get_start(self.children[1].clangcursor)
            op1_end = get_end(self.children[0].clangcursor)
            self.value = find_clang_token_between(self.GetTokens(), op1_end, op2_begin)
            # Sanity exception, should never hit
            if self.value is None: raise Exception("No binary operator found")

        # Nodes we never want to break down into values
        if self.nodetype == CursorKind.NULL_STMT:
            self.value = self.nodetype.name
        
        # Some funky cases we can have nodes with no values or tokens
        if self.value == None or self.value == '':
            self.value = self.nodetype.name
        
        if hasattr(self.value, "spelling"):
            self.value = self.value.spelling

    def SetLeafNodeValue(self):
        # Sometimes weird things end up as leaf nodes.  If we see any of
        # these just use the nodetype and not any of the tokens.
        if (
            # Nodes we never want to break down into values
            self.nodetype == CursorKind.NULL_STMT \
            # Multiple lines of code cramed into one block
            or self.nodetype == CursorKind.COMPOUND_STMT \
            # Type defs like (void, *)
            # Sometimes params don't have a name how to handle in leaves..
            # for example const char ** or something as we can't put ** as values
            #or self.nodetype == CursorKind.PARM_DECL \
            # Inline assembly.
            or self.nodetype == CursorKind.ASM_STMT \
            # These typically occur becuase some item is not present in libclang
            # Also possibly unresolved macros ex.. __format__,(,__printf__,,,2,,,3,)
            or self.nodetype == CursorKind.UNEXPOSED_ATTR \
            or self.nodetype == CursorKind.UNEXPOSED_STMT \
            or self.nodetype == CursorKind.UNEXPOSED_EXPR \
            # Breaks
            or self.nodetype == CursorKind.BREAK_STMT \
            # embeded functions?
            or self.nodetype == CursorKind.FUNCTION_DECL \
            # continue statment
            or self.nodetype == CursorKind.CONTINUE_STMT \
            # Return statement
            or self.nodetype == CursorKind.RETURN_STMT \
            # Not sure why these appear but there is a low freq for all of these
            or self.nodetype == CursorKind.IF_STMT \
            or self.nodetype == CursorKind.WHILE_STMT \
            or self.nodetype == CursorKind.DESTRUCTOR \
            or self.nodetype == CursorKind.SWITCH_STMT \
            or self.nodetype == CursorKind.CONSTRUCTOR \
            or self.nodetype == CursorKind.FOR_STMT \
            or self.nodetype == CursorKind.CXX_UNARY_EXPR):
            self.value = self.nodetype.name
        
        # If we don't have a value try token #1
        if self.value is None or self.value == '':
            gen = self.clangcursor.get_tokens()
            self.value = next(gen, None)
            if next(gen, None) is not None:
                t = [t.spelling for t in self.clangcursor.get_tokens()]
                msg = "More than one token in leaf: "+self.nodetype.name+"("+",".join(t)+")\n"
                sys.stderr.write(msg)
        
        # Some funky cases we can have nodes with no values or tokens
        if self.value == None or self.value == '':
            self.value = self.nodetype.name
        
        if hasattr(self.value, "spelling"):
            self.value = self.value.spelling

        # Clang sometimes stores the function name in a const attr under the function
        # if we are a child of the function_decl and our value is the functions name blank it out.
        if self.parent.nodetype == CursorKind.FUNCTION_DECL and self.parent.clangcursor.spelling == self.value:
            self.value = self.parent.nodetype.name
        
        # if we are a leaf, need to be only alphanumeric
        self.value = self.remove_pattern.sub('', self.value)

    def IsFunctionDefinition(self):
        if self.nodetype != CursorKind.FUNCTION_DECL: return None
        return self.clangcursor.is_definition()

    def ToCodePathNode(self):
        # There are a few nodes where we want to include more information than just the name
        if self.nodetype == CursorKind.BINARY_OPERATOR \
        or self.nodetype == CursorKind.COMPOUND_ASSIGNMENT_OPERATOR \
        or self.nodetype == CursorKind.UNARY_OPERATOR \
        or self.nodetype == CursorKind.CXX_UNARY_EXPR:
            return "("+self.nodetype.name+":"+self.value+")"
        return "("+self.nodetype.name+")"

    def ToOutputString(self):
        output = ('  ' * self.depth) + '-'+ ' '.join([self.nodetype.name, self.displayname, self.optype, self.value])
        return output

# Helper functions for dealing with clang nodes
def get_all_tokens(tokens):
    return [x.spelling for x in tokens]

def get_start(clangcursor):
   return clangcursor.extent.start

def get_end(clangcursor):
    return clangcursor.extent.end

def eq_clang_loc(loc1, loc2, loc1_mod = 0, loc2_mod=0 ):
    return (loc1.int_data+loc1_mod) == (loc2.int_data+loc2_mod)

def find_clang_token_by_start(tokens, start, start_mod):
    for t in tokens:
        if eq_clang_loc(start, t.location, start_mod, 0):
            return t.spelling

def find_clang_token_between(tokens, start, end):
    for t in tokens:
        if start.int_data <= t.extent.start.int_data and \
            end.int_data >= t.extent.end.int_data:
                return t.spelling
