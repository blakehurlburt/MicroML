//Autogenerated C++ Source Code DO NOT EDIT
#include <Arduino.h>
#include <runtime.h>

Obj* lambda_0(Obj* param, Environment *env) {
	env->push();
	env->bind("_", param);
	 Obj* ret = nullptr;
	if (unwrapBool(_lt_(makeRecord({std::make_pair("_0", env->lookup("n")),std::make_pair("_1", makeInt(2, env))}, env), env))) { 
		ret = makeInt(1, env);
	} else {
		ret = _mul_(makeRecord({std::make_pair("_0", env->lookup("n")),std::make_pair("_1", invoke("fact", _sub_(makeRecord({std::make_pair("_0", env->lookup("n")),std::make_pair("_1", makeInt(1, env))}, env), env), env))}, env), env);
	}
	ret->mark();
	env->pop();
	return ret;
}
Obj* fact(Obj* param, Environment *env) {
	env->push();
	env->bind("n", param);
	 Obj* ret = nullptr;
	ret = lambda_0(makeRecord({}, env), env);
	ret->mark();
	env->pop();
	return ret;
}
Obj* enter(Obj* param, Environment *env) {
	env->push();
	env->bind("_", param);
	 Obj* ret = nullptr;
	ret = invoke("fact", makeInt(8, env), env);
	ret->mark();
	env->pop();
	return ret;
}
Obj* _main_(Obj* param, Environment *env) {
	env->push();
	env->bind("_", param);
	 Obj* ret = nullptr;
	env->bind("fact", makeFun(*fact, new Environment(*env), env));
	env->bind("enter", makeFun(*enter, new Environment(*env), env));
	ret = invoke("enter", makeRecord({}, env), env);
	ret->mark();
	env->pop();
	return ret;
}
void setup() {
	Serial.begin(9600);
	while(!Serial);
	topEnv = new Environment();
	Serial.println(unwrapInt(_main_(makeRecord({}, topEnv), topEnv)));
}
void loop() {
}

