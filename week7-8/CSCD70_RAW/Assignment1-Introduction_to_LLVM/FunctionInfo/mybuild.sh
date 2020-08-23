rm *.o *.so
make -f Optimize.mk all
exit
# 早期粗制烂造的build
g++ -I/usr/lib/llvm-9/include -std=c++11   -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -fPIC  -c -o FunctionInfo.o FunctionInfo.cpp
g++ -dylib -shared -fPIC FunctionInfo.o -o FunctionInfo.so
clang -O0 -emit-llvm -c ./tests/loop.c -o ./tests/loop.bc
llvm-dis ./tests/loop.bc -o=./tests/loop.ll
opt -load ./FunctionInfo.so -function-info ./tests/loop.bc -o ./tests/loop-opt.bc
