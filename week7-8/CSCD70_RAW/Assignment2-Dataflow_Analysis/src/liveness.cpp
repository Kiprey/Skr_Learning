#include "dfa/framework.h"


namespace {

// 声明一个Variable类，便于打印
// 直接用Value* 输出会输出地址
class Variable
{
private:
    const Value * _val;
public:
    Variable(const Value * val) : _val(val) {}

    bool operator==(const Variable & val) const
    { return _val == val.getValue(); }

    const Value* getValue() const { return _val; }

    friend raw_ostream & operator<<(raw_ostream & outs, const Variable & val);
}; 

raw_ostream & operator<<(raw_ostream & outs, const Variable & var)
{
    outs << "[";  var._val->printAsOperand(outs, false);  outs << "]";
    return outs;
}

}  // namespace anonymous

// unordered_set需要自己定义一个hash函数
namespace std{
// Construct a hash code for 'Variable'.
template <>
struct hash <Variable> {
  std::size_t operator()(const Variable &var) const {
    std::hash <const Value *> value_ptr_hasher;

    std::size_t value_hash = value_ptr_hasher((var.getValue()));

    return value_hash;
  }
};
};

namespace {

/// @todo Implement @c Liveness using the @c dfa::Framework interface.
class Liveness final : public dfa::Framework < Variable, 
                        dfa::Direction::Backward >
{
protected:
    virtual BitVector IC() const override
    {
        // @DONE OUT[B] = \Phi（空集）
        return BitVector(_domain.size(), false);
    }
    virtual BitVector BC() const override
    {
        // @DONE OUT[ENTRY] = \Phi（空集）
        return BitVector(_domain.size(), false);
    }

    virtual BitVector MeetOp(const BasicBlock & bb) const override
    {
        // @DONE 此处应该是集合并运算后的结果
        BitVector result(_domain.size(), false);
        for(const BasicBlock* block : successors(&bb))
        {
            // 通常来讲，所有后驱基础块的第一条Instruction的IN集合就是当前基础块的OUT集
            const Instruction&  first_inst_in_block = block->front();
            BitVector curr_bv = _inst_bv_map.at(&first_inst_in_block);
            // 但这里要对含有phi指令的基础块作特殊处理
            for(auto phi_iter = block->phis().begin(); 
                phi_iter != block->phis().end(); phi_iter++)
            {
                const PHINode & phi_inst = *phi_iter;
                for(auto phi_inst_iter = phi_inst.block_begin(); 
                    phi_inst_iter != phi_inst.block_end(); phi_inst_iter++)
                {
                    // 获取PHI指令中的各个前驱基础块
                    BasicBlock* const &curr_bb = *phi_inst_iter;
                    // 如果当前前驱基础块不是现在的基础块
                    if(curr_bb != &bb)
                    {
                        const Value* curr_val = phi_inst.getIncomingValueForBlock(curr_bb);
                        // 如果当前值在domain中存在（避免常量等）
                        int idx = getDomainIndex(Variable(curr_val));
                        if(idx != -1)
                        {
                            // 将临时变量中对应变量的bit设置为false
                            assert(curr_bv[idx] = true);
                            curr_bv[idx] = false;
                        }
                    }
                }
            }
            // 与临时变量做集合并操作
            result |= curr_bv;
        }
        return result;
    }
    virtual bool TransferFunc(const Instruction & inst,
                  const BitVector & ibv,
                  BitVector & obv) override
    {
        // 注意：此时的ibv传入的是out集合，obv传入的是in集
        // @DONE 计算单个指令的IN集合
        // 注：使用getDomainIndex函数时，其参数中的Variable会隐式构造，不必手动调用

        BitVector new_obv = ibv;
        
        // use操作
        for(auto iter = inst.op_begin(); iter != inst.op_end(); iter++)
        {
            const Value* val = dyn_cast<Value>(*iter);
            assert(val != NULL);
            // 如果当前Variable存在于domain
            if(_domain.find(val) != _domain.end())
                new_obv[getDomainIndex(val)] = true;
        }
        // def操作。不是所有的指令都会定值，例如ret，所以要设置条件判断
        if(getDomainIndex(&inst) != -1)
            new_obv[getDomainIndex(&inst)] = false;   
        
        bool hasChanged = new_obv != obv;
        obv = new_obv;
        return hasChanged;
    }
    virtual void InitializeDomainFromInstruction(const Instruction & inst) override
    {
        for(auto iter = inst.op_begin(); iter != inst.op_end(); iter++)
            if (isa < Instruction > (*iter) || isa < Argument > (*iter))
                _domain.emplace(Variable(*iter));
    }
public:
    static char ID;

    Liveness() : dfa::Framework < domain_element_t, 
                       direction_c > (ID) {}
    virtual ~Liveness() override {}
};

char Liveness::ID = 1; 
RegisterPass < Liveness > Y ("liveness", "Liveness");

} // namespace anonymous
