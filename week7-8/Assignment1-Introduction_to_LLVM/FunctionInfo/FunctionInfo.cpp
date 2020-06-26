#include "llvm-9/llvm/Pass.h"
#include "llvm-9/llvm/IR/Module.h"
#include "llvm-9/llvm/Support/raw_ostream.h"

using namespace llvm;


namespace {

class FunctionInfo final : public ModulePass
{
public:
	static char ID;

	FunctionInfo() : ModulePass(ID) {}
	virtual ~FunctionInfo() override {}

  	// We don't modify the program, so we preserve all analysis.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const override
	{
		AU.setPreservesAll();
	}

	virtual bool runOnModule(Module & M) override
	{
		outs() << "CSCD70 Functions Information Pass" << "\n";
		bool transformed = false;

		// @TODO Please complete this function.

		// 遍历内含的所有函数
		for (auto iter = M.begin(); iter != M.end(); ++iter)
		{
			Function &func = *iter;
			outs() << "Name:" << func.getName() << "\n";
			outs() << "Number of Arguments: " << func.arg_size() << "\n";
			outs() << "Number of Direct Call Sites in the same LLVM module: "
				<< func.getNumUses() << "\n";
			outs() << "Number of Basic Blocks: " << func.size() << "\n";
			outs() << "Number of Instructions: " << func.getInstructionCount() << "\n";
		}
		
		return false;
	}
};

char FunctionInfo::ID = 0;
RegisterPass < FunctionInfo > X (
	"function-info",
	"CSCD70: Functions Information");

}  // namespace anonymous
