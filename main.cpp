#include <iostream>

using namespace std;

struct Expression;
struct Number;
struct BinaryOperation;
struct Visitor;

struct Expression
{
    Expression() = default;
    virtual ~Expression() = default;
    virtual double evaluate(void) const = 0;
    virtual void visit(const Visitor * visitor) const = 0;
};
struct Visitor
{
    virtual void visitNumber(const Number * number) const = 0;
    virtual void visitBinaryOperation(const BinaryOperation * binOp) const = 0;
    ~Visitor() = default;
};

struct Number : Expression
{
    Number(double number) : number_(number) {}
    double evaluate(void) const override
    {   return number_;     }
    double getNumber(void) const noexcept
    {   return number_;     }
    void visit(const Visitor * visitor) const override
    {   visitor->visitNumber(this); }
private:
    double number_ = 0;
};

struct BinaryOperation : Expression
{
    BinaryOperation(const Expression* left, const Expression* right, char op)
    : left_(left), right_(right), op_(op) {}

    double evaluate(void) const override
    {
        switch (op_)
        {
        case '+': return left_->evaluate() + right_->evaluate();
        case '-': return left_->evaluate() - right_->evaluate();
        case '*': return left_->evaluate() * right_->evaluate();
        case '/': return left_->evaluate() / right_->evaluate();
        default: return 0;
        }
    }
    const Expression* getLeft() const noexcept
    {   return left_;   }
    const Expression* getRight() const noexcept
    {   return right_;  }
    char getOp() const noexcept
    {   return op_;     }
    void visit(const Visitor * visitor) const override
    {   visitor->visitBinaryOperation(this); }
private:
    const Expression* left_;
    const Expression* right_;
    char op_;
};

struct VisitorImpl : Visitor
{
    void visitNumber(const Number * number) const
    {   cout << number->getNumber();    }
    void visitBinaryOperation(const BinaryOperation * binOp) const
    {
        cout << "(";
        binOp->getLeft()->visit(this);
        cout << ")" << binOp->getOp() << "(";
        binOp->getRight()->visit(this);
        cout << ")";
    }
};

int main()
{
    BinaryOperation bo1(new Number(1.11), new Number(2.22), '+');
    BinaryOperation bo2(new Number(5.5), new Number(5.5), '+');
    BinaryOperation root(&bo1, &bo2, '+');

    cout << root.evaluate() << endl;
    cout << sizeof(Number) << endl; /// class contain implicit "vptr"

    VisitorImpl vimpl;
    root.visit(&vimpl);
    cout << endl;

    Number num(42); /// check "vptr" type in others/same classes
    cout << ((*(int64_t *)(&bo1)) == *(int64_t *)(&num)) << endl;
    cout << ((*(int64_t *)(&bo1)) == *(int64_t *)(&bo2)) << endl;
    return 0;
}
