#ifndef ARDUINO_BINDINGS_H
#define ARDUINO_BINDINGS_H

#include <Arduino.h>
#include "runtime.h"

void println__(Obj* o) {
  if(o->type() == "int")
    Serial.println(unwrapInt(o));

  else if (o->type() == "real")
    Serial.println(unwrapReal(o));

  else if (o->type() == "record")
    Serial.println(unwrapString(o));

}

void pinMode__(int pin, int mode) {
  pinMode(pin, mode);
}

void digitalWrite__(int pin, int value) {
  digitalWrite(pin, value);
}

char read__() {
  return Serial.read();
}


void delay__(int ms) {
  delay(ms);
}

#endif
