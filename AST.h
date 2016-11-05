#include <string>
#include <list>

class ASTNode {
public:
    virtual std::string toString() = 0;
    virtual ~ASTNode() {
    }
};

class StatementNode: public ASTNode {
public:
  std::string toString() {
    return "{StatementNode}";
  };

  virtual ~StatementNode() {
  }
};

class ExpressionNode: public ASTNode {
public:
    std::string toString() {
      return "{ExpressionNode}";
    };

    ~ExpressionNode() {
    }
};

class BlockNode: public ASTNode {
public:

    BlockNode(std::list < StatementNode* > stmnts) : statements(stmnts){
    }

    std::string toString() {
        std::string str = "{BlockNode: ";
        for(StatementNode * s : statements) {
            str += s->toString() + " ";
        }
        str += "}";
        return str;
    }

    ~BlockNode() {
      for(StatementNode * s : statements) {
          delete s;
      }
    }

    std::list < StatementNode* > statements;
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

class IdentifierNode: public ExpressionNode {
public:
    IdentifierNode(std::string val) {
        id = val;
    }

    std::string toString() {
        return "{IdentifierNode: " + id + "}";
    }

    std::string id;
};

class BinOpNode: public ExpressionNode {
public:
    BinOpNode(IdentifierNode * op, ExpressionNode * left, ExpressionNode * right)
        : operation(op), lhs(left), rhs(right) {
    }

    std::string toString() {
        return "{BinOpNode: " + lhs->toString() + " " + operation->toString()  + " " + rhs->toString() + "}";
    }

    ~BinOpNode() {
      delete operation;
      delete lhs;
      delete rhs;
    }

    IdentifierNode * operation;
    ExpressionNode * lhs;
    ExpressionNode * rhs;

};

class InvokeNode: public ExpressionNode {
public:
    InvokeNode(IdentifierNode * id, ExpressionNode * expr) : fun(id), args(expr) {

    }

    std::string toString() {
        return "{InvokeNode: " + fun->toString() + " " + args->toString() + "}";
    }

    ~InvokeNode(){
      delete fun;
      delete args;
    }

    IdentifierNode * fun;
    ExpressionNode * args;
};

class IfNode: public ExpressionNode {
public:
    IfNode(ExpressionNode * cond, ExpressionNode * left, ExpressionNode * right)
        : condition(cond), ifthen(left), ifelse(right) {
    }

    std::string toString() {
        return "{IfNode: " + condition->toString() + "?" + ifthen->toString()  + ":" + ifelse->toString() + "}";
    }

    ~IfNode(){
      delete condition;
      delete ifthen;
      delete ifelse;
    }

    ExpressionNode * condition;
    ExpressionNode * ifthen;
    ExpressionNode * ifelse;
};

class FunDeclNode: public StatementNode {
public:
    FunDeclNode(IdentifierNode * id, IdentifierNode * p, ExpressionNode * expr)
      : fun(id), param(p), body(expr) {
    }

    std::string toString() {
        return "{FunDeclNode: " + fun->toString() + " " + param->toString() + " = " + body->toString() +  "}";
    }

    ~FunDeclNode() {
      delete fun;
      delete param;
      delete body;
    }

    IdentifierNode * fun;
    IdentifierNode * param;
    ExpressionNode * body;
};

class ValNode: public StatementNode {
public:
    ValNode(IdentifierNode * idtr, ExpressionNode * expr) : id(idtr), val(expr) {

    }

    std::string toString() {
        return "{ValNode: " + id->toString() + " = " + val->toString() +  "}";
    }

    ~ValNode() {
      delete id;
      delete val;
    }

    IdentifierNode * id;
    ExpressionNode * val;
};

class LambdaNode: public ExpressionNode {
public:
    LambdaNode(IdentifierNode * p, ExpressionNode * expr) : param(p), body(expr) {

    }

    std::string toString() {
        return "{LambdaNode: " + param->toString() + " => " + body->toString() +  "}";
    }

    ~LambdaNode() {
      delete param;
      delete body;
    }

    IdentifierNode * param;
    ExpressionNode * body;
};


class LetNode: public ExpressionNode {
public:
    LetNode(BlockNode * decls, ExpressionNode * expr) : let(decls), in(expr) {

    }

    std::string toString() {
        return "{LetNode: " + let->toString() + " in " + in->toString() +  "}";
    }

    ~LetNode() {
      delete let;
      delete in;
    }

    BlockNode * let;
    ExpressionNode * in;
};
