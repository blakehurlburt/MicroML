#ifndef RUNTIME_H
#define RUNTIME_H

#include <list>
#include <map>
#include <stdexcept>
#include <iostream>

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

    std::string type() const {
        return "record";
    }

    std::map < std::string, const Obj * > r;
};

class Fun: public Obj {
public:
    Fun(const Obj * (*func)(const Obj*, Environment*), Environment * closureEnv) {
        fun = func;
        env = closureEnv;
    }

    std::string type() const {
        return "function";
    }

    const Obj* (*fun)(const Obj*, Environment*);
    Environment *env;
};

class Bool: public Obj {
public:
    Bool(bool val) {
        value = val;
    }

    std::string type() const {
        return "boolean";
    }

    bool value;
};

class Int: public Obj {
public:
    Int(int val) {
        value = val;
    }

    virtual std::string type() const {
        return "integer";
    }

    int value;
};

class Real: public Obj {
public:
    Real(double val) {
        value = val;
    }

    std::string type() const {
        return "real";
    }

    double value;
};

class Char: public Obj {
public:
    Char(char val) {
        value = val;
    }

    std::string type() const {
        return "char";
    }

    char value;
};

bool unwrapBool(const Obj * o) {
    if(o->type() != "boolean") {
        throw std::runtime_error("not a boolean");
    }
    return ((Bool*)o)->value;
}

int unwrapInt(const Obj * o) {
    if(o->type() != "integer") {
        throw std::runtime_error("not an integer, is a " + o->type());
    }
    return ((Int*)o)->value;
}

const Obj* makeInt(int i) {
    return new Int(i);
}

const Obj* makeReal(double d) {
    return new Real(d);
}

const Obj* makeBool(bool b) {
    return new Bool(b);
}

const Obj* makeChar(char c) {
    return new Char(c);
}


const Obj* makeRecord(std::list < std::pair < std::string, const Obj * >> pairs) {
    return new Rec(pairs);
}

const Obj* makeFun(const Obj * (*func)(const Obj*, Environment*), Environment* closureEnv) {
    return new Fun(func, closureEnv);
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
        for(auto i = envs.begin(); i != envs.end(); ++i) {
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

const Obj* invoke(std::string id, const Obj* arg, Environment* env) {
    const Obj* o = env->lookup(id);
    if(o->type() != "function") {
        throw std::runtime_error("not a function");
    }

    Fun* f = ((Fun*)o);

    Environment * closureEnv = f->env;
    closureEnv->push();
    closureEnv->bind(id, env->lookup(id));
    const Obj * ret = (*(f->fun))(arg,closureEnv);
    closureEnv->pop();
    return ret;
}

const Obj* _add_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((const Int*)r0)->value + ((const Int*)r1)->value);
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

const Obj* _sub_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((const Int*)r0)->value - ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((const Int*)r0)->value - ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value - ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((const Real*) r0)->value - ((const Real*) r1)->value);
    }

    throw std::runtime_error("sub expression not valid");
    return nullptr;
}

const Obj* _mul_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((const Int*)r0)->value * ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((const Int*)r0)->value * ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value * ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((const Real*) r0)->value * ((const Real*) r1)->value);
    }

    throw std::runtime_error("mul expression not valid");
    return nullptr;
}

const Obj* _div_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((const Int*)r0)->value / ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((const Int*)r0)->value / ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value / ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((const Real*) r0)->value / ((const Real*) r1)->value);
    }

    throw std::runtime_error("div expression not valid");
    return nullptr;
}

const Obj* _lt_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((const Int*)r0)->value < ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((const Int*)r0)->value < ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value < ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((const Real*) r0)->value < ((const Real*) r1)->value);
    }

    throw std::runtime_error("div expression not valid");
    return nullptr;
}

const Obj* _gt_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((const Int*)r0)->value > ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((const Int*)r0)->value > ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value > ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((const Real*) r0)->value > ((const Real*) r1)->value);
    }

    throw std::runtime_error("div expression not valid");
    return nullptr;
}

const Obj* _ge_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((const Int*)r0)->value >= ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((const Int*)r0)->value >= ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value >= ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((const Real*) r0)->value >= ((const Real*) r1)->value);
    }

    throw std::runtime_error("div expression not valid");
    return nullptr;
}

const Obj* _le_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((const Int*)r0)->value <= ((const Int*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((const Int*)r0)->value <= ((const Real*) r1)->value);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value <= ((const Int*)r0)->value);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((const Real*) r0)->value <= ((const Real*) r1)->value);
    }

    throw std::runtime_error("div expression not valid");
    return nullptr;
}

bool ObjEq(const Obj* o0, const Obj* o1) {
  if (o0->type() != o1-> type())
    return false;

  if (o0->type() == "record") {
    auto r0 = ((Rec*)o0)->r;
    auto r1 = ((Rec*)o1)->r;

    for(auto e: r0) {
      auto entry = r1.find(e.first);
      if (entry == r1.end()) return false;
      if (!ObjEq(entry->second, e.second)) return false;
    }
    return true;
  }

if (o1->type() != "record")
  if(o0->type() == "integer" && o1->type() == "integer") {
      return makeBool(((const Int*)o0)->value == ((const Int*)o1)->value);
  }

  if(o0->type() == "integer" && o1->type() == "real") {
      return makeBool(((const Int*)o0)->value == ((const Real*)o1)->value);
  }

  if(o0->type() == "real" && o1->type() == "integer") {
      return makeBool(((Real*) o0)->value == ((const Int*)o0)->value);
  }

  if(o0->type() == "real" && o1->type() == "real") {
      return makeBool(((const Real*) o0)->value == ((const Real*)o1)->value);
  }

  if(o0->type() == "boolean" && o1->type() == "boolean") {
      return makeBool(((const Bool*) o0)->value == ((const Bool*)o1)->value);
  }

  if(o0->type() == "char" && o1->type() == "char") {
      return makeBool(((const Char*) o0)->value == ((const Char*)o1)->value);
  }

  return false;
}

const Obj* _eq_(const Obj* o, Environment* env) {
  if(o->type() != "record") {
      throw std::runtime_error("not a record");
  }

  const Obj* r0 = ((Rec*)o)->get("_0");
  const Obj* r1 = ((Rec*)o)->get("_1");
  return makeBool(ObjEq(r0, r1));
}

const Obj* _ne_(const Obj* o, Environment* env) {
  if(o->type() != "record") {
      throw std::runtime_error("not a record");
  }

  const Obj* r0 = ((Rec*)o)->get("_0");
  const Obj* r1 = ((Rec*)o)->get("_1");
  return makeBool(!ObjEq(r0, r1));
}

const Obj* _or_(const Obj* o, Environment* env) {
    if(o->type() != "record") {
        throw std::runtime_error("not a record");
    }

    const Obj* r0 = ((Rec*)o)->get("_0");
    const Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((const Bool*)r0)->value || ((const Bool*)r1)->value);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((const Bool*)r0)->value || ((const Bool*) r1)->value);
    }

    throw std::runtime_error("div expression not valid");
    return nullptr;
}

#endif
