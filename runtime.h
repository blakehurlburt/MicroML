#ifndef RUNTIME_H
#define RUNTIME_H

#include <list>
#include <map>
#include <stdexcept>

class Environment;

class Obj {
public:
    virtual std::string type() const {
        return "object";
    }
};

class Rec: public Obj {
public:
    Rec(std::list < std::pair < std::string, const Obj * >> pairs) {
        for(std::pair < std::string, const Obj * > p : pairs) {
            r.insert(p);
        }
    }

    const Obj* get(std::string name) const {
        if(r.count(name) == 0) {
            throw std::runtime_error("not found");
        }
        return r.find(name)->second;
    }

    std::string type() {
        return "record";
    }

    std::map < std::string, const Obj * > r;
};

class Fun: public Obj {
public:
    Fun(Obj * (*func)(Obj*, Environment*)) {
        fun = func;
    }

    std::string type() {
        return "function";
    }

    Obj* (*fun)(Obj*, Environment*);
};

class Bool: public Obj {
public:
    Bool(bool val) {
        value = val;
    }

    std::string type() {
        return "boolean";
    }

    bool value;
};

class Int: public Obj {
public:
    Int(int val) {
        value = val;
    }

    std::string type() {
        return "integer";
    }

    int value;
};

class Real: public Obj {
public:
    Real(double val) {
        value = val;
    }

    std::string type() {
        return "real";
    }

    double value;
};

const Obj* makeInt(int i) {
    return new Int(i);
}

const Obj* makeReal(double d) {
    return new Real(d);
}

const Obj* makeBool(bool b) {
    return new Bool(b);
}

const Obj* makeRecord(std::list < std::pair < std::string, const Obj * >> pairs) {
    return new Rec(pairs);
}

const Obj* makeFun(Obj * (*func)(Obj*, Environment*)) {
    return new Fun(func);
}

class Environment {
public:
    Environment() {
        push();
    }

    Environment(const Environment &that) {
        envs = that.envs;
    }

    void push() {
        envs.push_front(std::map < std::string, const Obj * > ());
    }

    void pop() {
        envs.pop_front();
    }

    const Obj* lookup(std::string name) {
        auto i = envs.begin();
        while(i != envs.end()) {
            if(i->count(name) != 0) {
                return i->find(name)->second;
            }
        }
        throw std::runtime_error("not found");
    }

    void bind(std::string name, const Obj* val) {
        envs.front()[name] = val;
    }
private:
    std::list < std::map < std::string, const Obj * >> envs;
};

const Obj* _add_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((const Int*)r0)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((const Int*)r0)->value + ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value + ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((const Real*) r0)->value + ((const Real*) r1)->value);
    }

    throw std::runtime_error("addition expression not valid");
    return nullptr;
}

#endif
