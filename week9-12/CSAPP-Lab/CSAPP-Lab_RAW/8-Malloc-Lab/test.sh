#!/bin/bash
echo amptjp-bal.rep
./mdriver -f traces/amptjp-bal.rep
echo binary2-bal.rep
./mdriver -f traces/binary2-bal.rep
echo binary-bal.rep
./mdriver -f traces/binary-bal.rep
echo cccp-bal.rep
./mdriver -f traces/cccp-bal.rep
echo coalescing-bal.rep
./mdriver -f traces/coalescing-bal.rep
echo cp-decl-bal.rep
./mdriver -f traces/cp-decl-bal.rep
echo expr-bal.rep
./mdriver -f traces/expr-bal.rep
echo random2-bal.rep
./mdriver -f traces/random2-bal.rep
echo random-bal.rep
./mdriver -f traces/random-bal.rep
echo realloc-bal.rep
./mdriver -f traces/realloc-bal.rep
echo realloc2-bal.rep
./mdriver -f traces/realloc2-bal.rep
