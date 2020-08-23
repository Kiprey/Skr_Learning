#include <llvm/Pass.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;


namespace {

class Transform final : public ModulePass
{
private:
	bool runOnBasicBlock(BasicBlock & B)
	{
		// get the first and second instruction
		Instruction & _1st_inst =    *B.begin(),
		            & _2nd_inst = *(++B.begin());
		// =============================================================
		// output the address of the 1st instruction
		outs() << &_1st_inst << "\n";
		// output the address of the 1st operand of the 2nd instruction
		outs() <<  _2nd_inst.getOperand(0) << "\n";

		// print the 1st instruction
		outs() << _1st_inst << "\n";
		// print the 1st instruction as an operand
		_1st_inst.printAsOperand(outs(), false); outs() << "\n";
		// =============================================================
		// User-Use-Value
		// =============================================================
		for (auto iter  = _1st_inst.op_begin();
		          iter != _1st_inst.op_end(); ++iter)
		{
			Value * operand = *iter;

			if (Argument * arg = dyn_cast < Argument > (operand))
			{
				outs() << "I am function " << arg->getParent()->getName() << "\'s #" <<
					arg->getArgNo() << " argument." << "\n";
			}
			if (ConstantInt * c = dyn_cast < ConstantInt > (operand))
			{
				outs() << "I am a constant integer of value " << c->getValue() << "\n";
			}
		}

		for (auto iter  = _1st_inst.user_begin(); 
		          iter != _1st_inst.user_end(); ++iter)
		{
			outs() << *(dyn_cast < Instruction > (*iter)) << "\n";
		}
		
		for (auto iter  = _1st_inst. use_begin();
		          iter != _1st_inst. use_end(); ++iter)
		{
			outs() << *(dyn_cast < Instruction > (iter->getUser())) << "\n";
		}
		// =============================================================
		// Instruction Manipulation
		// =============================================================
		Instruction * new_inst = BinaryOperator::Create(Instruction::Add, 
			_1st_inst.getOperand(0), _1st_inst.getOperand(0));
		
		new_inst->insertAfter(&_1st_inst);
		// Q: Is there any alternative to updating each reference separately?
		//    Please check the documentation and try answering this.
		// Q: What happens if we update the use references WITHOUT the insertion?
		_1st_inst.user_begin()->setOperand(0, new_inst);

		return true;
	}
	
	bool runOnFunction(Function & F)
	{
		bool transformed = false;
		
		for (auto iter = F.begin(); iter != F.end(); ++iter)
		{
			if (runOnBasicBlock(*iter))
			{
				transformed = true;
			}
		}
		
		return transformed;
	}
	
public:
	static char ID;

	Transform() : ModulePass(ID) {}
	virtual ~Transform() override {}

	// This time, we do modify the program, however, we preserve the CFG.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		AU.setPreservesCFG();
	}
  
	virtual bool runOnModule(Module & M)
	{
		bool transformed = false;
		
		for (auto iter = M.begin(); iter != M.end(); ++iter)
		{
			if (runOnFunction(*iter))
			{
				transformed = true;
			}
		}
		return transformed;
	}
};

char Transform::ID = 0;
RegisterPass < Transform > X (
	"transform",
	"Example Transform Pass"); 

}  // namespace anonymous
