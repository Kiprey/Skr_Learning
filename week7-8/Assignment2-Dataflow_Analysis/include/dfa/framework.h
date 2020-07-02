#pragma once

#include <cassert>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include <llvm/Pass.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace dfa {
/// Analysis Direction, used as Template Parameter
enum class Direction { Forward, Backward };

/// Dataflow Analysis Framework
/// 
/// @tparam TDomainElement  Domain Element
/// @tparam TDirection      Direction of Analysis
template < typename TDomainElement, Direction TDirection >
class Framework : public FunctionPass
{
/// This macro selectively enables methods depending on the analysis direction.
///
/// @param dir  Direction of Analysis
/// @param ret  Return Type
/// @comment 此处使用了std::enable_if，若条件满足则返回特定类型。若条件不满足则会报错
//              但泛型编程中存在一个最优匹配原则，这个模板不成立，就去推断其他模板，
//              所以相应的method实际上是一个重载函数
#define METHOD_ENABLE_IF_DIRECTION(dir, ret)                    \
    template < Direction _TDirection = TDirection >             \
    typename std::enable_if < _TDirection == dir, ret > ::type


/*
/// This macro does typedef depending on the analysis direction. If the
/// direction in the template argument does not match the argument, the
/// resulting type is not defined.
///
/// @param type_name  Name of the Resulting Type
/// @param dir    Direction of Analysis
/// @param T      Type Definition if `TDirection == dir`
 /// @comment 这个宏定义有非常大的问题，不予使用

#define TYPEDEF_IF_DIRECTION(type_name, dir, T)                 \
    using type_name = typename std::enable_if < TDirection == dir, T > ::type
*/

/// @todo For ∀ @c METHOD_ENABLE and @c TYPEDEF , you will have to add the
///       equivalent definition for backward analysis.
protected:
    typedef TDomainElement domain_element_t;
    static constexpr Direction direction_c = TDirection;
    /***********************************************************************
     * Domain
     ***********************************************************************/
    std::unordered_set < TDomainElement > _domain;
    /***********************************************************************
     * Instruction-BitVector Mapping
     ***********************************************************************/
    /// Mapping from Instruction Pointer to BitVector // @comment OUT集合
    std::unordered_map < const Instruction *, BitVector > _inst_bv_map;
    /// @brief Return the initial condition.
    /// 
    /// @todo  Override this method in every child class.
    virtual BitVector IC() const = 0;
    /// @brief Return the boundary condition.
    /// 
    /// @todo  Override this method in every child class.
    virtual BitVector BC() const = 0;
private:
    /// @brief Dump the domain under @p mask . E.g., If @c domian = {%1, %2,
    ///    %3,}, dumping it with @p mask = 001 will give {%3,}.
    void printDomainWithMask(const BitVector & mask) const
    {
        outs() << "{";

        assert(mask.size() == _domain.size() &&
               "The size of mask must be equal to the size of domain.");
        
        unsigned mask_idx = 0;
        for (const auto & elem : _domain)
        {
            if (!mask[mask_idx++])
            {
                continue;
            }
            outs() << elem << ", ";
        }  // for (mask_idx ∈ [0, mask.size()))
        outs() << "}";
    }
    // METHOD_ENABLE_IF_DIRECTION(Direction::Forward, void)
    // @comment 修改了一下这个函数，这样就可以复用了
    void printInstBV(const Instruction & inst) const
    {
        const BasicBlock * const pbb = inst.getParent();
        if (&inst == &(*InstTraversalOrder(*pbb).begin()))
        {
            // 让编译器自己去推导类型，因为这个类型有两种可能
            auto meet_operands = MeetOperands(*pbb);
            
            // If the list of meet operands is empty, then we are at
            // the boundary, hence print the BC.
            if (meet_operands.begin() == meet_operands.end())
            {
                outs() << "BC:\t";
                printDomainWithMask(BC());
                outs() << "\n";
            }
            else
            {
                outs() << "MeetOp:\t";
                printDomainWithMask(MeetOp(*pbb));
                outs() << "\n";
            }
        }
        outs() << "Instruction: " << inst << "\n";
        outs() << "\t";
        printDomainWithMask(_inst_bv_map.at(&inst));
        outs() << "\n";
    }
protected:
    /// @brief Dump, ∀inst ∈ @p F, the associated bitvector.
    void printInstBVMap(const Function & F) const
    {
        outs() << "********************************************" << "\n";
        outs() << "* Instruction-BitVector Mapping         " << "\n";
        outs() << "********************************************" << "\n";

        for (const auto & inst : instructions(F))
        {
            printInstBV(inst);
        }
    }
    /***********************************************************************
     * Meet Operator and Transfer Function
     ***********************************************************************/
    /*
    TYPEDEF_IF_DIRECTION(meetop_const_range,
                 Direction::Forward,
                 pred_const_range);
    */
    /// @brief Return the operands for the meet operation.
    METHOD_ENABLE_IF_DIRECTION(Direction::Forward, pred_const_range)
    MeetOperands(const BasicBlock & bb) const
    {
        return predecessors(&bb);
    }
    /*
    TYPEDEF_IF_DIRECTION(meetop_const_range,
                 Direction::Backward,
                 succ_const_range);
    */
    // DONE 为Backward建立的函数
    /// @brief Return the operands for the meet operation.
    METHOD_ENABLE_IF_DIRECTION(Direction::Backward, succ_const_range)
    MeetOperands(const BasicBlock & bb) const
    {
        return successors(&bb);
    }

    /// @brief  Apply the meet operation to a range of @p meet_operands .
    /// 
    /// @return the Resulting BitVector after the Meet Operation
    /// 
    /// @todo   Override this method in every child class.
    /// @comment backward处需要用到当前基础块，所以干脆把函数参数类型改了
    virtual BitVector MeetOp(const BasicBlock & bb) const = 0;
    /// @brief  Apply the transfer function at instruction @p inst to the
    ///     input bitvector to get the output bitvector.
    ///
    /// @return True if @p obv has been changed, False otherwise
    /// 
    /// @todo   Override this method in every child class.
    virtual bool TransferFunc(const Instruction & inst,
                  const BitVector & ibv,
                  BitVector & obv) = 0;
    /***********************************************************************
     * CFG Traversal
     ***********************************************************************/
private:
    /*
    TYPEDEF_IF_DIRECTION(bb_traversal_const_range,
                 Direction::Forward,
                 iterator_range < Function::const_iterator > );
    TYPEDEF_IF_DIRECTION(inst_traversal_const_range,
                 Direction::Forward,
                 iterator_range < BasicBlock::const_iterator > );
    */
    /// @brief Return the traversal order of the basic blocks.
    ///
    /// @todo  Modify the implementation (and the above typedef accordingly)
    ///    for the optimal traversal order.
    METHOD_ENABLE_IF_DIRECTION(Direction::Forward, 
        iterator_range < SymbolTableList<BasicBlock> ::const_iterator >)
    BBTraversalOrder(const Function & F) const
    {
        return make_range(F.begin(), F.end());
    }
    /// @brief Return the traversal order of the instructions.
    METHOD_ENABLE_IF_DIRECTION(Direction::Forward, 
        iterator_range < SymbolTableList<Instruction> ::const_iterator > )
    InstTraversalOrder(const BasicBlock & bb) const
    {
        return make_range(bb.begin(), bb.end());
    }
    /*
    TYPEDEF_IF_DIRECTION(bb_traversal_const_range,
                 Direction::Forward,
                 iterator_range < Function::const_reverse_iterator > );
    TYPEDEF_IF_DIRECTION(inst_traversal_const_range,
                 Direction::Forward,
                 iterator_range < BasicBlock::const_reverse_iterator > );
    */
    METHOD_ENABLE_IF_DIRECTION(Direction::Backward, 
        iterator_range < SymbolTableList<BasicBlock> ::const_reverse_iterator >)
    BBTraversalOrder(const Function & F) const
    {
        return make_range(F.getBasicBlockList().rbegin(), F.getBasicBlockList().rend());
    }
    /// @brief Return the traversal order of the instructions.
    METHOD_ENABLE_IF_DIRECTION(Direction::Backward, 
        iterator_range < SymbolTableList<Instruction> ::const_reverse_iterator > )
    InstTraversalOrder(const BasicBlock & bb) const
    {
        return make_range(bb.getInstList().rbegin(), bb.getInstList().rend());
    }
protected:
    /// @brief  返回特定类（Expression、Variable）在Domain中的偏移量
    /// 
    /// @return 如果存在，则返回相应的偏移量。不存在则返回-1
    int getDomainIndex(const TDomainElement& elem) const
    {
        //std::unordered_set < TDomainElement > _domain;
        auto inst_dom_iter = _domain.find(elem);
        if(inst_dom_iter == _domain.end())
            return -1;

        int idx = 0; 
        for(auto dom_iter = _domain.begin(); inst_dom_iter != dom_iter; ++dom_iter)
            ++idx;
        return idx;
    }

    /// @brief  Traverse through the CFG and update @c inst_bv_map .
    /// 
    /// @return True if changes are made to @c inst_bv_map, False otherwise
    /// 
    /// @todo   Implement this method for all the child classes.
    /// @comment 尽管这个函数是为Forward方向设计的，
    ///     但由于BBTraversalOrder以及InstTraversalOrder。这个函数同样可以供Backward使用
    bool traverseCFG(const Function & func)
    {
        bool transform = false;
        
        for(const BasicBlock& basicBlock : BBTraversalOrder(func))
        {
            // OUT[B]和IN[B]的节点集合
            BitVector ibv;
            BitVector obv;
            // 确定IN集合
            // 注：basicBlock不存在operator==函数，故只能比较其地址是否相同
            if(&basicBlock == &(*BBTraversalOrder(func).begin()))
                // 设置OUT[ENTRY]为空集，就是设置EntryBlock的IN集为空集
                // 注意基础块ENTRY和此处函数开头的基础块不是同一个
                // ENTRY -> funcEntryBlock -> ... -> funcExitBlock -> EXIT
                ibv = BC();
            else 
                ibv = MeetOp(basicBlock);
            // 利用fB=fsn⋅…⋅fs2⋅fs1公式，遍历Instruction，计算出当前基础块的OUT集合
            for(const Instruction& inst : InstTraversalOrder(basicBlock))
            {
                // 对_inst_bv_map的修改在TransferFunc内
                // 传入旧的obv，判断是否修改out集。
                obv = _inst_bv_map[&inst];
                // TransferFunc的第三个参数感觉有点多余
                transform |= TransferFunc(inst, ibv, obv);
                // 计算出的inst_out集合，是下一次transfer的in集合，所以需要赋值
                ibv = obv;
            }
        }
        return transform;
    }
    
public:
    Framework(char ID) : FunctionPass(ID) {}
    virtual ~Framework() override {}

    // We don't modify the program, so we preserve all analysis.
    virtual void getAnalysisUsage(AnalysisUsage & AU) const
    {
        AU.setPreservesAll();
    }
protected:
    /// @brief Initialize the domain from each instruction.
    /// 
    /// @todo  Override this method in every child class.
    virtual void InitializeDomainFromInstruction(const Instruction & inst) = 0;
public:
    virtual bool runOnFunction(Function & F) override final
    {
        for (const auto & inst : instructions(F))
        {
            InitializeDomainFromInstruction(inst);
        }
        for (const auto & inst : instructions(F))
        {
            _inst_bv_map.emplace(&inst, IC());
        }
        // keep traversing until changes have been made to the
        // instruction-bv mapping

        while (traverseCFG(F)) {}

        printInstBVMap(F);

        return false;
    }
#undef METHOD_ENABLE_IF_DIRECTION
};

}  // namespace dfa
