#include <string>
#include <list>

class ASTNode {
public:
    std::string toString() {
        return "{ASTNode}";
    }
};

class StatementNode: public ASTNode {
public:
    std::string toString() {
        return "{StatementNode}";
    }
};

class ExpressionNode: public ASTNode {
public:
    std::string toString() {
        return "{ExpNode}";
    }
};

class BlockNode: public ASTNode {
public:

    BlockNode(std::list < StatementNode > stmnts) : statements(stmnts){

    }

    std::string toString() {
        std::string str = "{BlockNode: ";
        for(StatementNode s : statements) {
            str += s.toString() + " ";
        }
        str += "}";
        return str;
    }

    std::list < StatementNode > statements;
};

class IntNode: public ExpressionNode {
public:
    IntNode(int val) {
        value = val;
    }

    std::string toString() {
        return "{IntNode: " + std::to_string(value) + "}";
    }

    int value;
};

class RealNode: public ExpressionNode {
public:
    RealNode(double val) {
        value = val;
    }

    std::string toString() {
        return "{RealNode: " + std::to_string(value) + "}";
    }

    double value;
};

class IdentfierNode: public ExpressionNode {
public:
    IdentfierNode(std::string val) {
        id = val;
    }

    std::string toString() {
        return "{IdentfierNode: " + id + "}";
    }

    std::string id;
};

class BinOpNode: public ExpressionNode {
public:
    BinOpNode(IdentfierNode op, ExpressionNode left, ExpressionNode right)
        : operation(op), lhs(left), rhs(right) {
    }

    std::string toString() {
        return "{BinOpNode: " + lhs.toString() + " " + operation.toString()  + " " + rhs.toString() + "}";
    }

    IdentfierNode operation;
    ExpressionNode lhs;
    ExpressionNode rhs;

};

class InvokeNode: public ExpressionNode {
public:
    InvokeNode(IdentfierNode id, ExpressionNode expr) : fun(id), args(expr) {

    }

    std::string toString() {
        return "{InvokeNode: " + fun.toString() + " " + args.toString() + "}";
    }

    IdentfierNode fun;
    ExpressionNode args;
};

class IfNode: public ExpressionNode {
public:
    IfNode(ExpressionNode cond, ExpressionNode left, ExpressionNode right)
        : condition(cond), ifthen(left), ifelse(right) {
    }

    std::string toString() {
        return "{IfNode: " + condition.toString() + "?" + ifthen.toString()  + ":" + ifelse.toString() + "}";
    }

    ExpressionNode condition;
    ExpressionNode ifthen;
    ExpressionNode ifelse;
};

class FunDeclNode: public StatementNode {
public:
    FunDeclNode(IdentfierNode id, IdentfierNode p, ExpressionNode expr) : fun(id), param(p), body(expr) {

    }

    std::string toString() {
        return "{FunDeclNode: " + fun.toString() + " " + param.toString() + " = " + body.toString() +  "}";
    }

    IdentfierNode fun;
    IdentfierNode param;
    ExpressionNode body;
};

class ValNode: public StatementNode {
public:
    ValNode(IdentfierNode idtr, ExpressionNode expr) : id(idtr), val(expr) {

    }

    std::string toString() {
        return "{ValNode: " + id.toString() + " = " + val.toString() +  "}";
    }

    IdentfierNode id;
    ExpressionNode val;
};

class LambdaNode: public ExpressionNode {
public:
    LambdaNode(IdentfierNode p, ExpressionNode expr) : param(p), body(expr) {

    }

    std::string toString() {
        return "{LambdaNode: " + param.toString() + " => " + body.toString() +  "}";
    }

    IdentfierNode param;
    ExpressionNode body;
};


class LetNode: public ExpressionNode {
public:
    LetNode(BlockNode decls, ExpressionNode expr) : let(decls), in(expr) {

    }

    std::string toString() {
        return "{LetNode: " + let.toString() + " in " + in.toString() +  "}";
    }

    BlockNode let;
    ExpressionNode in;
};
