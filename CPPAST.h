#ifndef CPPAST_H
#define CPPAST_H

#include <string>
#include <list>

class CPPStatement {
public:
    virtual ~CPPStatement() {

    }
    virtual std::string genCode() {
        return ";";
    }
};


class CPPExpression {
public:
    virtual ~CPPExpression() {

    }
    virtual std::string genCode() {
        return "";
    }
};

class CPPIntExpression: public CPPExpression {
public:
    CPPIntExpression(int val) {
        value = val;
    }

    virtual std::string genCode() {
        return "mInt(" + std::to_string(value) + ")";
    }

    int value;
};

class CPPBoolExpression: public CPPExpression {
public:
    CPPBoolExpression(bool val) {
        value = val;
    }

    virtual std::string genCode() {
        return "Bool(" + std::to_string(value) + ")";
    }

    bool value;
};


class CPPRealExpression: public CPPExpression {
public:
    CPPRealExpression(double val) {
        value = val;
    }

    virtual std::string genCode() {
        return "Real(" + std::to_string(value) + ")";
    }

    double value;
};

class CPPIfExpression: public CPPExpression {
public:
    CPPIfExpression(CPPExpression * cond, CPPExpression * left, CPPExpression * right)
        : condition(cond), ifthen(left), ifelse(right) {
    }

    ~CPPIfExpression(){
        delete condition;
        delete ifthen;
        delete ifelse;
    }

    virtual std::string genCode() {
        return "ifexpr(" + condition->genCode() + "," + ifthen->genCode() + "," + ifelse->genCode() + ")";
    }

    CPPExpression * condition;
    CPPExpression * ifthen;
    CPPExpression * ifelse;
};

class CPPInvokeExpression: public CPPExpression {
public:
    CPPInvokeExpression(std::string f, CPPExpression * a) : fun(f), arg(a) {
    }

    virtual std::string genCode() {
        return "invoke(\"" + fun + "\", " + arg->genCode() + ", env)";
    }

    std::string fun;
    CPPExpression* arg;
};


class CPPBinding: public CPPStatement {
public:
    CPPBinding(std::string id, CPPExpression * e) : name(id), expr(e) {
    }

    ~CPPBinding() {
        delete expr;
    }

    virtual std::string genCode() {
        return "env->bind(\"" + name + "\", " + expr->genCode() + ");";
    }

    std::string name;
    CPPExpression* expr;
};

class CPPReturn: public CPPStatement {
public:
    CPPReturn(CPPExpression * e) : expr(e) {
    }

    ~CPPReturn() {
        delete expr;
    }

    virtual std::string genCode() {
        return "obj ret = " + expr->genCode() + ";";
    }

    CPPExpression* expr;
};

class CPPFunction {
public:

    CPPFunction(std::string n, std::string p, std::list < CPPStatement * > b)
        : name(n), param(p), body(b) {

    }

    ~CPPFunction() {
        for(CPPStatement* s : body) {
            delete s;
        }
    }

    virtual std::string genCode() {
        std::string fun = "obj " + name + "(obj param, Environment *env) {\n";
        fun += "\tenv.push()\n";
        fun += "env.bind(\"" + param + "\", param)";
        for(CPPStatement* statement : body) {
            fun += "\t" + statement->genCode() + "\n";
        }
        fun += "\tenv.pop();";
        fun += "\treturn ret;";
        fun += "}\n";
        return fun;
    }

    std::string name;
    std::string param;
    std::list < CPPStatement * > body;
};

#endif
