OPTIMIZER := Transform.so
OPT_PASSES := -transform

LLVM_VERSION := 6.0

CXXFLAGS := $(shell llvm-config-$(LLVM_VERSION) --cxxflags)

OPT_OBJs := $(subst .cpp,.o,$(wildcard *.cpp))

TEST_SRCs := $(basename $(notdir $(wildcard ./tests/*.c)))
TEST_RAW_LLs := $(subst .c,.ll,$(wildcard ./tests/*.c))
TEST_OPT_LLs := $(addprefix ./tests/,$(addsuffix -opt.ll,$(TEST_SRCs)))

all: $(TEST_OPT_LLs) $(TEST_RAW_LLs) 

./tests/%-opt.ll: ./tests/%-opt.bc
	llvm-dis-$(LLVM_VERSION) $< -o=$@
	
./tests/%.ll: ./tests/%-m2r.bc
	llvm-dis-$(LLVM_VERSION) $< -o=$@

./tests/%-opt.bc: ./tests/%-m2r.bc $(OPTIMIZER)
	env LD_LIBRARY_PATH=. opt-$(LLVM_VERSION) -load $(OPTIMIZER) $(OPT_PASSES) $< -o $@

./tests/%-m2r.bc: ./tests/%.bc
	opt-$(LLVM_VERSION) -mem2reg $< -o $@

./tests/%.bc: ./tests/%.c
	clang-$(LLVM_VERSION) -O0 -Xclang -disable-O0-optnone -emit-llvm -c $< -o $@

$(OPTIMIZER): $(OPT_OBJs)
	$(CXX) -dylib -shared $^ -o $@

.PHONY: clean
clean:
	$(RM) $(TEST_OPT_LLs) $(TEST_RAW_LLs) $(OPTIMIZER) $(OPT_OBJs)
