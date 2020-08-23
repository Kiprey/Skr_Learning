#include <llvm/Support/raw_ostream.h>

#include "analysis.h"

using namespace llvm;


class Analysis;  // Forward Declaration

namespace {

class AnotherTransform final : public ModulePass
{	
public:
	static char ID;

	AnotherTransform() : ModulePass(ID) {}
	virtual ~AnotherTransform() override {}

	virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		AU.addRequired  < Analysis > ();
		AU.addPreserved < Analysis > ();

		AU.setPreservesCFG();
	}  
	virtual bool runOnModule(Module & M)
	{
		outs() << "Another Transform" << "\n";

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

char AnotherTransform::ID = 2;
RegisterPass < AnotherTransform > Z (
	"another-transform",
	"Another Transform");

}  // namespace anonymous
