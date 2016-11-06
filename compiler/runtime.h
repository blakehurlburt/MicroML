#ifndef RUNTIME_H
#define RUNTIME_H

extern "C" {
int _getpid(){
    return -1;
}
int _kill(int pid, int sig){
    return -1;
}
int _write(){
    return -1;
}
}

#include <list>
#include <map>
#include <vector>
#include <string>
#include <Arduino.h>

class Obj;

std::vector < Obj * > gcList;

const char* toString(int i) {
    return String(i).c_str();
}

void _error_(std::string msg) {
    Serial.print("error: ");
    Serial.println(msg.c_str());
    pinMode(13, OUTPUT);
    while(true) {
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
        delay(500);
    }
}

class Obj {
public:
    virtual std::string type() {
        return "object";
    }

    virtual ~Obj() {
    }

    bool gcFlag = false;
    bool deleted = false;

    virtual void mark() {
        gcFlag = true;
    }

    bool sweep() {
        bool wasCollected = !gcFlag;
        if(!gcFlag) {
            delete this;
            //deleted = true;
        }
        gcFlag = false;
        return wasCollected;
    }
};

class Int: public Obj {
public:
    Int(int val) {
        value = val;
    }

    ~Int(){

    }

    virtual void mark() {
        gcFlag = true;
    }

    virtual std::string type() {
        return "integer";
    }

    int value;
};

class Environment;
Environment* topEnv = nullptr;

class Environment {
public:
    Environment() {
        push();
    }

    void push() {
        envs.push_front(std::map < std::string, Obj * > ());
    }

    void mark() {
        for(auto i : envs) {
            for(auto p : i) {
                p.second->mark();
            }
        }
    }

    void GC() {
        mark();
        int freed = 0;
        for(int i = 0; i < gcList.size(); ++i) {
            if(gcList[i]->sweep() && gcList.size() > 1) {
                gcList[i] = gcList[gcList.size() - 1];
                gcList.pop_back();
                --i;
                freed++;
            }
        }
    }

    void pop() {
        if(!envs.empty()) {
            envs.pop_front();
        }

        topEnv->GC();
    }

    Obj* lookup(std::string name) {
        for(auto i = envs.begin(); i != envs.end(); ++i) {
            if(i->count(name) != 0) {
                return i->find(name)->second;
            }
        }
        _error_("not found: " + name);
        return nullptr;
    }

    void bind(std::string name, Obj* val) {
        if(name == "") {
            // Serial.print((String("__") + String((int)val)).c_str());
            // Serial.print(":");
            // if(val->type() == "integer") {
            //     Serial.println(((Int*)val)->value);
            // } else {
            //     Serial.println(val->type().c_str());
            // }
            envs.front()[(String("__") + String((int)val)).c_str()] = val;
        } else {
            envs.front()[name] = val;
            // Serial.print(name.c_str());
            // Serial.print(":");
            // if(val->type() == "integer") {
            //     Serial.println(((Int*)val)->value);
            // } else {
            //     Serial.println(val->type().c_str());
            // }
        }
    }
private:
    std::list < std::map < std::string, Obj * >> envs;
};

class Rec: public Obj {
public:
    Rec(std::list < std::pair < std::string, Obj * >> pairs) {
        for(std::pair < std::string, Obj * > p : pairs) {
            r.insert(p);
        }
    }

    ~Rec(){

    }

    Obj* get(std::string name) {
        if(r.count(name) == 0) {
            _error_("not found");
        }
        return r.find(name)->second;
    }

    std::string type() {
        return "record";
    }

    void mark() {

        if(gcFlag) {
            return;
        }
        gcFlag = true;

        for(std::pair < std::string, Obj * > p : r) {
            p.second->mark();
        }
    }

    std::map < std::string, Obj * > r;
};

class Fun: public Obj {
public:
    Fun(Obj * (*func)(Obj*, Environment*), Environment * closureEnv) {
        fun = func;
        env = closureEnv;
    }

    ~Fun() {
        //delete env;
    }

    std::string type() {
        return "function";
    }

    void mark() {

        if(gcFlag) {
            return;
        }
        gcFlag = true;

        env->mark();
    }

    Obj* (*fun)(Obj*, Environment*);
    Environment *env;
};

class Bool: public Obj {
public:
    Bool(bool val) {
        value = val;
    }

    ~Bool(){

    }

    std::string type() {
        return "boolean";
    }

    bool value;
};

class Real: public Obj {
public:
    Real(double val) {
        value = val;
    }

    ~Real(){

    }

    std::string type() {
        return "real";
    }

    double value;
};

class Char: public Obj {
public:
    Char(char val) {
        value = val;
    }

    ~Char(){

    }

    std::string type() {
        return "char";
    }

    char value;
};

bool unwrapBool(Obj * o) {
    if(o->type() != "boolean") {
        _error_("not a boolean");
    }
    return ((Bool*)o)->value;
}

int unwrapInt(Obj * o) {
    if(o->type() != "integer") {
        _error_("not an integer, is a " + o->type());
    }
    return ((Int*)o)->value;
}

char unwrapChar(const Obj * o) {
  if (o->type() != "char")
    _error_("not a char, is a " + o->type());
  return ((Char*) o)->value;
}

std::string unwrapString(const Obj * o) {
    if(o->type() != "record") {
        _error_("not a string, is a " + o->type());
    }
    Rec* rec = (Rec*) o;
    std::string result = "";
    while (!rec->r.empty()) {
      result += unwrapChar(rec->get("head"));
      rec = rec->get("tail");
    }

    return result;
}

std::string unwrapList(const Obj * o) {
  if(o->type() != "record") {
      _error_("not a list, is a " + o->type());
  }
  Rec* rec = (Rec*) o;
  std::string result = "[";
  Object* next;
  while (!rec->r.empty()) {
    next = rec->get("head");
    if (next->type() == "integer")
      result += std::to_string(unwrapInt(next));
    else if (next->type() == "real")
      result += std::to_string(unwrapReal(next));
    else if (next->type() == "char")
      result += std::to_string(unwrapChar(next));
    else if (next->type() == "record")
      result += std::to_string(unwrapList(next));

    rec = rec->get("tail");
  }

  return result + "}";
}

Obj* makeInt(int i, Environment* env) {
    Obj * o = new Int(i);
    gcList.push_back(o);
    env->bind("",o);
    return o;
}

Obj* makeReal(double d, Environment* env) {
    Obj* o = new Real(d);
    gcList.push_back(o);
    env->bind("",o);
    return o;
}

Obj* makeBool(bool b, Environment* env) {
    Obj* o = new Bool(b);
    gcList.push_back(o);
    env->bind("",o);
    return o;
}

Obj* makeChar(char c, Environment* env) {
    Obj* o = new Char(c);
    gcList.push_back(o);
    env->bind("",o);
    return o;
}

Obj* makeRecord(std::list < std::pair < std::string, Obj * >> pairs, Environment* env) {
    Obj* o = new Rec(pairs);
    gcList.push_back(o);
    env->bind("",o);
    return o;
}

Obj* makeFun(Obj * (*func)(Obj*, Environment*), Environment* closureEnv, Environment* env) {
    Obj* o = new Fun(func, closureEnv);
    gcList.push_back(o);
    env->bind("",o);
    return o;
}

Obj* invoke(std::string id, Obj* arg, Environment* env) {
    Obj* o = env->lookup(id);
    if(o->type() != "function") {
        _error_("not a function");
    }

    Fun* f = ((Fun*)o);

    Environment * closureEnv = f->env;
    closureEnv->push();
    closureEnv->bind(id, env->lookup(id));
    Obj * ret = (*(f->fun))(arg,closureEnv);
    ret->mark();
    closureEnv->pop();
    return ret;
}

Obj* _get_(Obj* o, std::string field, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
        return nullptr;
    }

    auto r1 = ((Rec*)o);
    auto entry = r1->r.find(field);

    if(entry == r1->r.end()) {
        _error_("no such field");
        return nullptr;
    }

    return entry->second;

}

Obj* _add_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((Int*)r0)->value + ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((Int*)r0)->value + ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value + ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((Real*) r0)->value + ((Real*) r1)->value, env);
    }

    _error_("addition expression not valid");
    return nullptr;
}

Obj* _sub_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((Int*)r0)->value - ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((Int*)r0)->value - ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value - ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((Real*) r0)->value - ((Real*) r1)->value, env);
    }

    _error_("sub expression not valid");
    return nullptr;
}

Obj* _mul_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((Int*)r0)->value * ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((Int*)r0)->value * ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value * ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((Real*) r0)->value * ((Real*) r1)->value, env);
    }

    _error_("mul expression not valid");
    return nullptr;
}

Obj* _div_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeInt(((Int*)r0)->value / ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeReal(((Int*)r0)->value / ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeReal(((Real*) r0)->value / ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeReal(((Real*) r0)->value / ((Real*) r1)->value, env);
    }

    _error_("div expression not valid");
    return nullptr;
}

Obj* _lt_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((Int*)r0)->value < ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((Int*)r0)->value < ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value < ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((Real*) r0)->value < ((Real*) r1)->value, env);
    }

    _error_("div expression not valid");
    return nullptr;
}

Obj* _gt_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((Int*)r0)->value > ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((Int*)r0)->value > ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value > ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((Real*) r0)->value > ((Real*) r1)->value, env);
    }

    _error_("div expression not valid");
    return nullptr;
}

Obj* _ge_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((Int*)r0)->value >= ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((Int*)r0)->value >= ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value >= ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((Real*) r0)->value >= ((Real*) r1)->value, env);
    }

    _error_("div expression not valid");
    return nullptr;
}

Obj* _le_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((Int*)r0)->value <= ((Int*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((Int*)r0)->value <= ((Real*) r1)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "integer") {
        return makeBool(((Real*) r0)->value <= ((Int*)r0)->value, env);
    }

    if(r0->type() == "real" && r1->type() == "real") {
        return makeBool(((Real*) r0)->value <= ((Real*) r1)->value, env);
    }

    _error_("div expression not valid");
    return nullptr;
}

bool ObjEq(Obj* o0, Obj* o1, Environment* env) {
    if(o0->type() != o1->type()) {
        return false;
    }

    if(o0->type() == "record") {
        auto r0 = ((Rec*)o0)->r;
        auto r1 = ((Rec*)o1)->r;

        for(auto e: r0) {
            auto entry = r1.find(e.first);
            if(entry == r1.end()) {
                return false;
            }
            if(!ObjEq(entry->second, e.second, env)) {
                return false;
            }
        }
        return true;
    }

    if(o1->type() != "record") {
        if(o0->type() == "integer" && o1->type() == "integer") {
            return ((Int*)o0)->value == ((Int*)o1)->value;
        }
    }

    if(o0->type() == "integer" && o1->type() == "real") {
        return ((Int*)o0)->value == ((Real*)o1)->value;
    }

    if(o0->type() == "real" && o1->type() == "integer") {
        return ((Real*) o0)->value == ((Int*)o0)->value;
    }

    if(o0->type() == "real" && o1->type() == "real") {
        return ((Real*) o0)->value == ((Real*)o1)->value;
    }

    if(o0->type() == "boolean" && o1->type() == "boolean") {
        return ((Bool*) o0)->value == ((Bool*)o1)->value;
    }

    if(o0->type() == "char" && o1->type() == "char") {
        return ((Char*) o0)->value == ((Char*)o1)->value;
    }

    return false;
}

Obj* _eq_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");
    return makeBool(ObjEq(r0, r1, env), env);
}

Obj* _ne_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");
    return makeBool(!ObjEq(r0, r1, env), env);
}

Obj* _or_(Obj* o, Environment* env) {
    if(o->type() != "record") {
        _error_("not a record");
    }

    Obj* r0 = ((Rec*)o)->get("_0");
    Obj* r1 = ((Rec*)o)->get("_1");

    if(r0->type() == "integer" && r1->type() == "integer") {
        return makeBool(((Bool*)r0)->value || ((Bool*)r1)->value, env);
    }

    if(r0->type() == "integer" && r1->type() == "real") {
        return makeBool(((Bool*)r0)->value || ((Bool*) r1)->value, env);
    }

    _error_("div expression not valid");
    return nullptr;
}

#endif
