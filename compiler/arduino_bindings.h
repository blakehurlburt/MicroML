#ifndef ARDUINO_BINDINGS_H
#define ARDUINO_BINDINGS_H

#include <Arduino.h>
#include "runtime.h"

void println__(Obj * o, Environment * env) {
  if(o->type() == "int")
    Serial.println(unwrapInt(o));

  else if (o->type() == "real")
    Serial.println(unwrapReal(o));

  else if (o->type() == "record") {
    Record* rec = (Record*) o;
    if (rec->r.empty() || rec->r.count("head") <= 0) return;

    if (rec->get("head").type() == "char")
      Serial.println(unwrapString(o).c_str());
    else
      Serial.println(unwrapList(o).c_str());
    }
  }

}

void pinMode__(Obj * o, Environment* env) {
  if (o->type() != "record")
    _error_("not a record");

  Rec* rec = (Rec*) o;
  Object* pin = rec->get("pin"),
          val = rec->get("mode");

  if (pin->type() != "integer" || mode->type() != "integer")
    _error_("arguments are not ints");

  pinMode(unwrapInt(pin), unwrapInt(mode));
}

void digitalWrite__(Obj * o, Environment* env) {
  if (o->type() != "record")
    _error_("not a record");

  Record* rec = (Record*) o;
  Object* pin = rec->get("pin"),
          val = rec->get("val");

  if (pin->type() != "integer" || val->type() != "integer")
    _error_("arguments are not ints");

  digitalWrite(unwrapInt(pin), unwrapInt(val));
}

Char* read__() {
  return makeChar(Serial.read());
}


void delay__(Object * o, Environment * env) {
  if (o.type() != "integer")
    __error__("not an integer");
  delay(unwrapInt(o));
}

#endif
