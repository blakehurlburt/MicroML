#include <string>
#include <list>
#include <iostream>
#include "CPPAST.h"

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

    virtual CPPStatement* translate(std::list < CPPFunction* > & funList) {
        return nullptr;
    }
};

class ExpressionNode: public ASTNode {
public:
    virtual std::string toString() {
        return "{ExpressionNode}";
    };

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return nullptr;
    }

    ~ExpressionNode() {
    }
};

class BlockNode: public ASTNode {
public:

    BlockNode(){
    }

    BlockNode(std::list < StatementNode * > stmnts) : statements(stmnts){
    }

    void add(StatementNode * s) {
        statements.push_back(s);
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

    std::list < StatementNode * > statements;
};

class IdentifierNode: public ExpressionNode {
public:
    IdentifierNode(std::string val) {
        id = val;
    }

    std::string toString() {
        return "{IdentifierNode: " + id + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPLookupExpression(id);
    }

    std::string id;
};

class IntNode: public ExpressionNode {
public:
    IntNode(int val) {
        value = val;
    }

    std::string toString() {
        return "{IntNode: " + std::to_string(value) + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPIntExpression(value);
    }

    int value;
};

class CharNode: public ExpressionNode {
public:
    CharNode(char val) {
        value = val;
    }

    std::string toString() {
        return "{CharNode: " + std::to_string(value) + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPCharExpression(value);
    }

    char value;
};

class RealNode: public ExpressionNode {
public:
    RealNode(double val) {
        value = val;
    }

    std::string toString() {
        return "{RealNode: " + std::to_string(value) + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPRealExpression(value);
    }

    double value;
};

class BoolNode: public ExpressionNode {
public:
    BoolNode(bool val) {
        value = val;
    }

    std::string toString() {
        return "{BoolNode: }" + std::to_string(value) + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPBoolExpression(value);
    }

    bool value;
};

class ValNode: public StatementNode {
public:
    ValNode(IdentifierNode * idtr, ExpressionNode * expr) : id(idtr), val(expr) {

    }

    std::string toString() {
        return "{ValNode: " + id->toString() + " = " + val->toString() +  "}";
    }

    virtual CPPStatement* translate(std::list < CPPFunction* > & funList) {
        return new CPPBinding(id->id, val->translate(funList));
    }

    ~ValNode() {
        delete id;
        delete val;
    }

    IdentifierNode * id;
    ExpressionNode * val;
};

class RecordNode: public ExpressionNode {
public:

    RecordNode() {
    }

    RecordNode(std::list < ValNode * >  val) : fields(val) {
    }

    void add(ValNode* v){
        fields.push_back(v);
    }

    std::string toString() {
        std::string str = "{RecordNode: ";
        for(ValNode* v : fields) {
            str += v->id->toString() + "=" + v->val->toString() + " ";
        }
        str += "}";
        return str;
    }

    ~RecordNode() {
        for(ValNode* v : fields) {
            delete v;
        }
    }
    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {

        std::list < std::pair < std::string, CPPExpression * >> l;

        for(ValNode* v : fields) {
            l.push_back(std::make_pair(v->id->id, v->val->translate(funList)));
        }

        return new CPPRecordExpression(l);
    }

    std::list < ValNode * > fields;
};

class GetNode: public ExpressionNode {
public:
    GetNode(ExpressionNode * expr, IdentifierNode * id) : rec(expr), field(id) {

    }

    std::string toString() {
        return "{GetNode: " + rec->toString() + "." + field->toString() + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPGetExpression(rec->translate(funList), field->id);
    }

    ~GetNode(){
        delete field;
        delete rec;
    }

    IdentifierNode * field;
    ExpressionNode * rec;
};

CPPExpression* buildList(std::list < CharNode * > ::const_iterator itr, std::list < CharNode * > ::const_iterator end, std::list < CPPFunction* > & funList) {

    if(itr == end) {
        return new CPPRecordExpression({});
    }

    return new CPPRecordExpression({
        std::make_pair("head", (*itr)->translate(funList)),
        std::make_pair("tail",buildList(++itr, end, funList))
    });

}

class StringNode: public ExpressionNode {
public:
    StringNode(std::string val) {
        value = val;
    }

    std::string toString() {
        return "{StringNode: \"" + value + "\"}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {

        std::list < CharNode * > charList;

        for(char c : value) {
            charList.push_back(new CharNode(c));
        }

        return buildList(charList.begin(), charList.end(), funList);

    }
    std::string value;
};

class ParamListNode: public ASTNode {
public:

    ParamListNode(IdentifierNode * i){
        add(i);
    }

    ~ParamListNode() {
        for(IdentifierNode * i : params) {
            delete i;
        }
    }

    void add(IdentifierNode* i) {
        params.push_back(i);
    }

    std::string toString() {
        std::string str = "{ParamListNode: ";
        for(IdentifierNode * i : params) {
            str += i->toString() + " ";
        }
        str += "}";
        return str;
    }

    std::list < IdentifierNode * > params;
};

class InvokeNode: public ExpressionNode {
public:
    InvokeNode(ExpressionNode * id, ExpressionNode * expr) : fun(id), args(expr) {

    }

    std::string toString() {
        return "{InvokeNode: " + fun->toString() + " " + args->toString() + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        return new CPPInvokeExpression(fun->translate(funList), args->translate(funList));
    }

    ~InvokeNode(){
        delete fun;
        delete args;
    }

    ExpressionNode * fun;
    ExpressionNode * args;
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

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {

        RecordNode rnode({ \
            new ValNode(new IdentifierNode("_0"), lhs), \
            new ValNode(new IdentifierNode("_1"), rhs) \
        });

        CPPExpression * ret = new CPPNativeExpression(operation->id, rnode.translate(funList));
        for(ValNode* v : rnode.fields) {
            v->val = nullptr;
        }
        return ret;
    }

    IdentifierNode * operation;
    ExpressionNode * lhs;
    ExpressionNode * rhs;

};

CPPFuncExpression* buildFunction(std::string name, std::list < IdentifierNode* > ::iterator begin,
                                 std::list < IdentifierNode* > ::iterator end, std::list < StatementNode * > body,
                                 ExpressionNode* ret, std::list < CPPFunction* > & funList) {
    //

    std::string param = (*begin)->id;
    std::list < CPPStatement * > statements;



    if(++begin == end) {

        for(StatementNode * s : body) {
            statements.push_back(s->translate(funList));
        }
        statements.push_back(new CPPReturn(ret->translate(funList)));

    } else {

        std::string lambdaNum = std::to_string(funList.size());
        funList.push_front(nullptr); //Just for lambda count to work correctly
        statements.push_back( \
            new CPPReturn( \
                buildFunction("lambda_" + lambdaNum, begin, end, body, ret, funList)));
        funList.pop_front(); //nullptr
    }

    funList.push_back(new CPPFunction(name, param, statements));
    return new CPPFuncExpression(name);

}

class IfNode: public ExpressionNode {
public:
    IfNode(ExpressionNode * cond, ExpressionNode * left, ExpressionNode * right)
        : condition(cond), ifthen(left), ifelse(right) {
    }

    std::string toString() {
        return "{IfNode: " + condition->toString() + "?" + ifthen->toString()  + ":" + ifelse->toString() + "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {
        std::list < CPPStatement * > statements;
        statements.push_back( \
            new CPPIfStatement(condition->translate(funList), \
                               new CPPReturn(ifthen->translate(funList)), \
                               new CPPReturn(ifelse->translate(funList))));
        std::string lambdaNum = std::to_string(funList.size());
        funList.push_back(new CPPFunction("lambda_" + lambdaNum, "_", statements));
        return new CPPNativeExpression("lambda_" + lambdaNum, new CPPRecordExpression({}));
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
    FunDeclNode(IdentifierNode * id, ParamListNode * p, ExpressionNode * expr)
        : fun(id), param(p), body(expr) {
    }

    std::string toString() {
        return "{FunDeclNode: " + fun->toString() + " " + param->toString() + " = " + body->toString() +  "}";
    }

    virtual CPPStatement* translate(std::list < CPPFunction* > & funList) {
        return new CPPBinding(fun->id, \
                              buildFunction(fun->id, param->params.begin(), param->params.end(), std::list < StatementNode * > (), body, funList));
    }

    ~FunDeclNode() {
        delete fun;
        delete param;
        delete body;
    }

    IdentifierNode * fun;
    ParamListNode * param;
    ExpressionNode * body;
};

class LambdaNode: public ExpressionNode {
public:
    LambdaNode(IdentifierNode * p, ExpressionNode * expr) : param(p), body(expr) {

    }

    std::string toString() {
        return "{LambdaNode: " + param->toString() + " => " + body->toString() +  "}";
    }

    virtual CPPExpression* translate(std::list < CPPFunction* > & funList) {

        std::list < CPPStatement * > statements;
        statements.push_back(new CPPReturn(body->translate(funList)));
        std::string lambdaNum = std::to_string(funList.size());
        funList.push_front(new CPPFunction("lambda_" + lambdaNum, param->id, statements));
        return new CPPFuncExpression("lambda_" + lambdaNum);
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
