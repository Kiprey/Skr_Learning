#include "dfa/framework.h"


namespace {

class Expression
{
private:
    unsigned _opcode; const Value * _lhs, * _rhs;
public:
    Expression(const Instruction & inst)
    {
        // @DONE 初始化
        // 在该实验中，只考察二元运算符。所以可以放心的初始化_rhs
        _opcode = inst.getOpcode();
        _lhs = inst.getOperand(0);
        _rhs = inst.getOperand(1);
    }

    bool operator==(const Expression & Expr) const
    {
        // @DONE 判断两个表达式是否相同
        switch(_opcode)
        {
            // 满足交换率
            case Instruction::Add:
            case Instruction::FAdd: // 浮点数加法
            case Instruction::Mul:
            case Instruction::FMul:
                return ((Expr.getLHSOperand() == _lhs  && Expr.getRHSOperand() == _rhs) 
                || (Expr.getLHSOperand() == _rhs && Expr.getRHSOperand() == _lhs)
                    && Expr.getOpcode() == _opcode);
            default: 
                return Expr.getLHSOperand() == _lhs && Expr.getRHSOperand() == _rhs
                    && Expr.getOpcode() == _opcode;
        }
    }

    unsigned getOpcode() const { return _opcode; }
    const Value * getLHSOperand() const { return _lhs; }
    const Value * getRHSOperand() const { return _rhs; }

    friend raw_ostream & operator<<(raw_ostream & outs, const Expression & expr);
}; 

raw_ostream & operator<<(raw_ostream & outs, const Expression & expr)
{
    outs << "[" << Instruction::getOpcodeName(expr._opcode) << " ";
    expr._lhs->printAsOperand(outs, false); outs << ", ";
    expr._rhs->printAsOperand(outs, false); outs << "]";

    return outs;
}

}  // namespace anonymous

namespace std {

// Construct a hash code for 'Expression'.
template <>
struct hash < Expression >
{
    std::size_t operator()(const Expression & expr) const
    {
        std::hash < unsigned > unsigned_hasher; std::hash < const Value * > pvalue_hasher;

        std::size_t opcode_hash = unsigned_hasher(expr.getOpcode());
        std::size_t lhs_operand_hash = pvalue_hasher((expr.getLHSOperand()));
        std::size_t rhs_operand_hash = pvalue_hasher((expr.getRHSOperand()));

        return opcode_hash ^ (lhs_operand_hash << 1) ^ (rhs_operand_hash << 1);
    }
};

}  // namespace std

namespace {

class AvailExpr final : public dfa::Framework < Expression, 
                        dfa::Direction::Forward >
{
protected:
    virtual BitVector IC() const override
    {
        // @DONE OUT[B] = U （全集）
        return BitVector(_domain.size(), true);
    }
    virtual BitVector BC() const override
    {
        // @DONE OUT[ENTRY] = \Phi（空集）
        return BitVector(_domain.size(), false);
    }
    // 函数参数类型改了
    virtual BitVector MeetOp(const BasicBlock & bb) const override
    {
        // @DONE 此处应该是集合交运算后的结果
        BitVector result(_domain.size(), true);
        for(const BasicBlock* block : predecessors(&bb))
        {
            // 所有前驱基础块的最后一条Instruction的OUT集合，就是整个基础块的IN集
            const Instruction&  last_inst_in_block = block->back();
            BitVector curr_bv = _inst_bv_map.at(&last_inst_in_block);
            result &= curr_bv;
        }
        return result;
    }
    virtual bool TransferFunc(const Instruction & inst,
                  const BitVector & ibv,
                  BitVector & obv) override
    {
        // @DONE 计算单个指令的out集合

        // 用于结尾处判断是否修改
        BitVector origin_obv = obv;
        // 对obv进行gen/kill操作，届时将obv赋值给map
        obv = ibv;

        // 只针对二元运算符进行操作
        if(isa<BinaryOperator> (inst))
        {
            // 将x op y generate
            obv[getDomainIndex(Expression(inst))] = true;
            // 将 z相关的表达式 kill
            // 事实上，这里不需要kill，因为LLVM的SSA（不过为了学习，代码还是写出来了）
            for(const User* user : inst.users())
                if(const Instruction* user_inst = dyn_cast<BinaryOperator>(user))
                    // 不需要验证user_inst是否存在于domain
                    // 因为domain已经添加了整个函数的二元指令
                    obv[getDomainIndex(Expression(*user_inst))] = false;
            /*
            公式：
            初始时，e_genB和e_killB都为空
            且
                e_genB = e_genB + "x op y" - "exprs(z)"
                e_killB = e_killB + "exprs(z)" - "x op y" 
            可以看出，每个Instruction中e_genB的改变量与（-e_killB，注意负号）是一样的
            所以可以推出 
                ==>> e_genB == -e_killB

            所以OUT[B] = e_genB U (IN[B] - e_killB)
                        = e_genB U (IN[B] + e_genB)
                        = IN[B] U e_genB
            于是后续就可以不再对e_kill集合进行分析

            注意，上述的几点是建立在Instrucion的基础之上。
            如果是以基本块为基础时，e_genB不一定会与 -e_killB 相同
            */
        }
        _inst_bv_map[&inst] = obv;
        return origin_obv != obv;
    }
    virtual void InitializeDomainFromInstruction(const Instruction & inst) override
    {
        if (isa < BinaryOperator > (inst))
        {
            // 相同的Expression将不会被重复添加
            _domain.emplace(inst);
        }
    }
public:
    static char ID;

    AvailExpr() : dfa::Framework < domain_element_t, 
                       direction_c > (ID) {}
    virtual ~AvailExpr() override {}
};

char AvailExpr::ID = 1; 
RegisterPass < AvailExpr > Y ("avail_expr", "Available Expression");

} // namespace anonymous
