#ifndef RUNTIME_H
#define RUNTIME_H

#include <list>
#include <map>
#include <stdexcept>

class Environment;
class Obj;

Obj _add_(Obj o, Environment* env) {
  if (o.type() != "record") throw std::runtime_error("not a record");
  Obj& r0 = ((Rec)o).get("_0");
  Obj& r1 = ((Rec)o).get("_1");

  if (r0.type() != "integer" && r0.type() != "real" || r1.type() != "integer" && r1.type() != "real")
     throw std::runtime_error("addition expression not valid");

  return r0.value() + r1.value();
}











class Obj {
  virtual std::string type() {
    return "object";
  }
};

class Rec : public Obj {
public:
  Rec(std::list<std::pair<std::string, Obj> > pairs) {
    for(std::pair<std::string, Obj> p : pairs)
      r.insert(p);
  }

  Obj get(std::string name) {
    if (r.count(name) == 0) throw std::runtime_error("not found");
    return r.find(name)->second;
  }

  std::string type() {
    return "record";
  }

  std::map<std::string, Obj> r;
};

class Fun : public Obj {
public:
  Fun(Obj (*func)(Obj, Environment*)) {
    fun = func;
  }

  std::string type() {
    return "function";
  }

  Obj (*fun)(Obj, Environment*);
};

class Bool : public Obj {
public:
  Bool(bool val) {
    value = val;
  }

  std::string type() {
    return "boolean";
  }

  bool value;
};

class Int : public Obj {
public:
  Int(int val) {
    value = val;
  }

  std::string type() {
    return "integer";
  }

  int value;
};

class Real : public Obj {
public:
  Real(double val) {
    value = val;
  }

  std::string type() {
    return "real";
  }

  double value;
};

class Environment {
public:
  Environment() {
    push();
  }

  Environment(const Environment& that) {
    envs = that.envs;
  }

  void push() {
    envs.push_front(std::map<std::string, Obj>());
  }

  void pop() {
    envs.pop_front();
  }

  Obj lookup(std::string name) {
    auto i = envs.begin();
    while(i != envs.end()) {
      if (i->count(name) != 0)
       return i->find(name)->second;
    }
    throw std::runtime_error("not found");
  }

  void bind(std::string name, Obj val) {
    envs.front()[name] = val;
  }
private:
  std::list<std::map<std::string, Obj> > envs;
};

#endif
