#ifndef CPPAST_H
#define CPPAST_H

#include <string>
#include <list>
#include <iostream>

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
        return "makeInt(" + std::to_string(value) + ")";
    }

    int value;
};

class CPPCharExpression: public CPPExpression {
public:
    CPPCharExpression(char val) {
        value = val;
    }

    virtual std::string genCode() {
        return "makeChar(" + std::to_string(value) + ")";
    }

    char value;
};

class CPPBoolExpression: public CPPExpression {
public:
    CPPBoolExpression(bool val) {
        value = val;
    }

    virtual std::string genCode() {
        return "makeBool(" + std::to_string(value) + ")";
    }

    bool value;
};


class CPPRealExpression: public CPPExpression {
public:
    CPPRealExpression(double val) {
        value = val;
    }

    virtual std::string genCode() {
        return "makeReal(" + std::to_string(value) + ")";
    }

    double value;
};

class CPPRecordExpression: public CPPExpression {
public:
    CPPRecordExpression(std::list < std::pair < std::string, CPPExpression * >> f) : fields(f) {

    }

    virtual std::string genCode() {
        std::string str =  "makeRecord({";
        for(std::pair < std::string, CPPExpression * > f : fields) {
            str += "std::make_pair(\"" + f.first + "\", " + f.second->genCode() + "),";
        }
        if(!fields.empty()) {
            str = str.substr(0,str.length() - 1);
        }
        str += "})";

        return str;
    }

    std::list < std::pair < std::string, CPPExpression * >> fields;
};

class CPPFuncExpression: public CPPExpression {
public:
    CPPFuncExpression(std::string name) : id(name) {
    }

    virtual std::string genCode() {
        return "makeFun(*" + id + ", new Environment(*env))";
    }

    std::string id;
};

class CPPIfStatement: public CPPStatement {
public:
    CPPIfStatement(CPPExpression * cond, CPPStatement * left, CPPStatement * right)
        : condition(cond), ifthen(left), ifelse(right) {
    }

    ~CPPIfStatement(){
        delete condition;
        delete ifthen;
        delete ifelse;
    }

    virtual std::string genCode() {
        return "if (unwrapBool(" + condition->genCode() + ")) { \n\t\t" \
               + ifthen->genCode() \
               + "\n\t} else {\n\t\t" \
               + ifelse->genCode() \
               + "\n\t}";
    }

    CPPExpression * condition;
    CPPStatement * ifthen;
    CPPStatement * ifelse;
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

class CPPNativeExpression: public CPPExpression {
public:
    CPPNativeExpression(std::string f, CPPExpression * a) : fun(f), arg(a) {
    }

    virtual std::string genCode() {
        return fun + "(" + arg->genCode() + ", env)";
    }

    std::string fun;
    CPPExpression* arg;
};

class CPPLookupExpression: public CPPExpression {
public:
    CPPLookupExpression(std::string id) : name(id) {
    }

    virtual std::string genCode() {
        return "env->lookup(\"" + name + "\")";
    }

    std::string name;
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
        return "ret = " + expr->genCode() + ";";
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
        std::string fun = "const Obj* " + name + "(const Obj* param, Environment *env) {\n";
        fun += "\tenv->push();\n";
        fun += "\tenv->bind(\"" + param + "\", param);\n";
        fun += "\tconst Obj* ret = nullptr;\n";
        for(CPPStatement* statement : body) {
            fun += "\t" + statement->genCode() + "\n";
        }
        fun += "\tenv->pop();\n";
        fun += "\treturn ret;\n";
        fun += "}\n";
        return fun;
    }

    std::string name;
    std::string param;
    std::list < CPPStatement * > body;
};

#endif
