OPTIMIZER := FunctionInfo.so

LLVM_VERSION ?= 9

CXXFLAGS := $(shell llvm-config-$(LLVM_VERSION) --cxxflags) -fPIC

OPT_OBJs := $(subst .cpp,.o,$(wildcard *.cpp))

all: $(OPTIMIZER) 

# Build the optimizer from the source files.
$(OPTIMIZER): $(OPT_OBJs)
	$(CXX) -dylib -fPIC -shared $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OPTIMIZER) $(OPT_OBJs)
