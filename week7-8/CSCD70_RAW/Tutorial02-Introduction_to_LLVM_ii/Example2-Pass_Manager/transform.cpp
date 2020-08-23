#include <llvm/Support/raw_ostream.h>

#include "analysis.h"

using namespace llvm;


namespace {

class Transform final : public ModulePass
{	
public:
	static char ID;

	Transform() : ModulePass(ID) {}
	virtual ~Transform() override {}

	virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		// If you comment this line out, the 'Analysis' pass will not be
		// run. LLVM will also give you a runtime error upon executing
		//
		//     std::vector < unsigned > my_stats = 
		//		getAnalysis < Analysis > ().getStats(); 
		//
		// (shown below in the `runOnModule` method) as you have not
		// requested pass `Analysis`.
		AU.addRequired  < Analysis > ();
		// If you comment this line out, the 'Analysis' pass will be run
		// once again on 'AnotherTransform'.
		AU.addPreserved < Analysis > ();

		AU.setPreservesCFG();
	}
	virtual bool runOnModule(Module & M)
	{
		outs() << "Transform" << "\n";

		std::vector < unsigned > my_stats = getAnalysis < Analysis > ().getStats();

		for (auto iter = my_stats.begin();
		     iter != my_stats.end(); ++iter)
		{
			outs() << *iter << ", ";
		}
		outs() << "\n";

		return true;
	}
};

char Transform::ID = 1;
RegisterPass < Transform > Y (
	"transform",
	"Transform");

}  // namespace anonymous
