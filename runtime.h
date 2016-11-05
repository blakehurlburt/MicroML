#ifndef RUNTIME_H
#define RUNTIME_H

#include <list>
#include <map>
#include <stdexcept>

class Environment;

class Obj {
  };

class Rec : public Obj {
public:
  Obj get(std::string name) {
    if (r.count(name) == 0) throw std::runtime_error("not found");
    return r.find(name)->second;
  }

  std::map<std::string, Obj> r;
};

class Fun : public Obj {

private:
  Obj (*fun)(Obj, Environment*);
};

class Bool : public Obj {
public:
  Bool(bool val) {
    value = val;
  }

  bool value;
};

class Int : public Obj {
public:
  Int(int val) {
    value = val;
  }

  int value;
};

class Real : public Obj {
public:
  Real(double val) {
    value = val;
  }
  double value;
};

class Environment {
  Environment() {
    push();
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
