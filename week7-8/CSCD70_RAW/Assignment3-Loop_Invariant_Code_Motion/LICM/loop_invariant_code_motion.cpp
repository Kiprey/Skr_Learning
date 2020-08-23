#include <llvm/Analysis/LoopPass.h>
#include <llvm/Analysis/ValueTracking.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

// 查找指令太长了，用宏缩小一下
#define FOUND(list, elem) \
    std::find(list.begin(), list.end(), elem) == list.end()

using namespace llvm;

namespace {

class LoopInvariantCodeMotion final : public LoopPass
{
private:
	DominatorTree * dom_tree;  // owned by `DominatorTreeWrapperPass`
    std::list<Instruction*> markedAsInvariant;
    
    // 检查当前指令是否是循环不变量
    bool isInvariant(Instruction * I, Loop* loop)
    {
        bool is_invariant = true;
        for(auto iter = I->op_begin(); is_invariant && iter != I->op_end(); ++iter)
        {
            Value* val = dyn_cast<Value>(*iter);
            assert(val != NULL);
            // 如果当前操作数不是一个常量，那可能是被标定为循环不变量或者其他
            // 如果是函数参数则可以视为循环体之外的不变量
            // 注意：这里的函数参数，指的是当前循环所在函数的函数参数，不是循环内部调用函数传入的参数
            if(!isa<Constant> (val) && !isa<Argument>(val))
            {
                if(Instruction* inst = dyn_cast<Instruction>(val))
                {
                    // 如果既不是循环不变量指令，也不是循环外的指令
                    if(FOUND(markedAsInvariant, inst)
                        && loop->contains(inst->getParent()))
                        // 那么该操作数就不是循环不变量
                        is_invariant = false;
                }
                else 
                {
                    // 不是所有遍历到的操作数都是指令、常量和函数参数，可能也有基本块的Label等等
                    // 其他情况下设置成false肯定没错
                    is_invariant = false;
                }
            }
        }
        return isSafeToSpeculativelyExecute(I)  // 检查未定义错误，例如除以0。
                                                // 这一条指令可以帮助过滤PHI和跳转指令
        && !I->mayReadFromMemory()              // 修改读取内存的指令，可能会导致结果值的改变，所以不予处理
        && !isa < LandingPadInst > (I)          // 异常处理相关的指令，必须在循环内部
        && is_invariant;                        
    }
    
    // 检查s所在的基本块是否是循环所有出口节点的支配节点
    bool isDomExitblocks(Instruction* inst, Loop* loop)
    {
        // getExitBlock只能返回一个BasicBlock，所以弃之不用
        // SmallVector第二个模板参数设置为0,以初始化内含的数组大小
        SmallVector<BasicBlock*, 0> exitblock;
        loop->getExitBlocks(exitblock);
        for(BasicBlock* bb : exitblock)
            // 只要有一个basicblock没有被inst的基础块所支配，那就返回false
            if(!dom_tree->dominates(inst->getParent(), bb))
                return false;
        return true;
    }
    // 检查循环中是否没有其它语句对x赋值
    bool AssignOnce(Instruction* inst)
    {
        // 由于SSA的特殊性，所有IR指令中，不存在其他语句会对某个"IR变量"进行不同位置的赋值
        return true;
    }
    // 检查循环中对x的引用是否仅由s到达
    bool OneWayToReferences(Instruction* inst)
    {
        // 由于SSA的特殊性，所有IR指令中，只有一条语句会引用某个"IR变量"结果。
        return true;
    }    
    // 将指令移动到前置首节点的末尾
    bool moveToPreheader(Instruction* inst, Loop* loop)
    {
        BasicBlock* preheader = loop->getLoopPreheader();
        // 都执行到这里了，当前循环的前置首节点不可能为空
        assert(preheader != NULL);
        /*  
            注意函数使用的是move，该函数会自动断开与原来基础块的连接，并移动到新的指令后面
            注意是移动到preheader的终结指令 *之前* 而不是 *之后* ，原因是
                1. 循环前置首节点最后一条指令是终结指令，移动到终结指令之后则肯定无法执行
                2. 最关键的一点，会报错，提示没有terminator
            基本块的终结指令基本上就是跳转指令

            同时，在这种情况下，不存在说，把指令移出来了，而该指令的操作数还在循环内部定值
        */
        inst->moveBefore(preheader->getTerminator());
        return false;
    }
public:
        static char ID;

	LoopInvariantCodeMotion() : LoopPass(ID) {}
	virtual ~LoopInvariantCodeMotion() override {}

    virtual void getAnalysisUsage(AnalysisUsage & AU) const
	{
		AU.addRequired < DominatorTreeWrapperPass > ();
        AU.addRequired < LoopInfoWrapperPass > ();
		AU.setPreservesCFG();
	}

    virtual bool runOnLoop(Loop * L, LPPassManager & LPM)
	{
        outs() << "ENTRY ################################################\n";
        // 如果前置首结点不存在，那就不优化
        if(!L->getLoopPreheader())
            return false;

		dom_tree = &(getAnalysis < DominatorTreeWrapperPass > ().getDomTree());
        // 测试了一下，发现该Pass内的变量存活至下一个循环，所以必须清空。
        markedAsInvariant.clear();
        // runOnLoop函数的返回值
        bool IR_has_Changed = false;
        // 将循环不变代码放入集合中
        bool hasChanged;
        do{
            hasChanged = false;
            // 遍历基础块
            for(BasicBlock* bb : L->blocks()){
                // loopinfo是当前loop的信息（不包括当前循环中的嵌套循环）
                LoopInfo* loopinfo = &(getAnalysis < LoopInfoWrapperPass > ().getLoopInfo());
                /* 
                    可能会遇到嵌套循环的情况。此时不对嵌套循环的指令做处理,
                    留给下一次嵌套循环调用当前pass了再处理
                    之所以判断处用了loopinfo->getLoopFor而不是loop->contain，
                        是因为一个是自下而上，另一个是自上而下
                */
                if(loopinfo->getLoopFor(bb) == L){
                    for(Instruction& inst : *bb){
                        // 如果先前该指令没有被标记，并且再此基础上还是循环不变变量
                        if(FOUND(markedAsInvariant, &inst)
                            && isInvariant(&inst, L)){
                            hasChanged = true;
                            markedAsInvariant.push_back(&inst);
                        }
                    } 
                }
            }
        }while(hasChanged);

        // 成功移动的指令个数
        int moveCount = 0;
        // 注意，要按照指令的顺序来判断与移动指令
        // 当初用std::list就是看重它的有序性
        for(Instruction* inst : markedAsInvariant)
        {
            /*
                移动代码的三个条件：
                    1. s所在的基本块是循环所有出口结点(有后继结点在循环外的结点)的支配结点
                    2. 循环中没有其它语句对x赋值
                    3. 循环中对语句s:x=y+z中，x的引用仅由s到达
            */
            if(isDomExitblocks(inst, L) && AssignOnce(inst) && OneWayToReferences (inst))
            {
                // 将指令移动到循环的前置首节点
                moveToPreheader(inst, L);
                outs() << "移出循环的指令 " << moveCount << " : " << *inst << "\n";
                IR_has_Changed = true;
                moveCount++;
            }
        }
        
        outs() << "循环不变量数量：\t\t" << markedAsInvariant.size() << "\n";
        outs() << "已移出循环的不变量数量：\t" <<  moveCount << "\n";
        outs() << "EXIT #################################################\n\n";
        return IR_has_Changed;
    }
};

char LoopInvariantCodeMotion::ID = 0;

RegisterPass < LoopInvariantCodeMotion > X (
	"loop-invariant-code-motion",
	"Loop Invariant Code Motion");

}  // namespace anonymous
