rm *.o *.so
make
exit

g++ -I/usr/lib/llvm-9/include -std=c++11   -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -fPIC  -c -o LocalOpts.o LocalOpts.cpp
g++ -dylib -shared -fPIC LocalOpts.o -o LocalOpts.so
clang -O0 -Xclang -disable-O0-optnone -emit-llvm -c ./tests/algebra.c -o ./tests/algebra.bc
llvm-dis ./tests/algebra.bc -o=./tests/algebra.ll
opt -mem2reg -load ./LocalOpts.so  -Local-Opts ./tests/algebra.ll -S -o ./tests/algebra-opt.ll 
