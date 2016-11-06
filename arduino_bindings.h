#ifndef ARDUINO_BINDINGS_H
#define ARDUINO_BINDINGS_H

#include<Arduino.h>
#include "runtime.h"

void Serial.println(Obj* o) {
  if(o->type() == "int") {

  }
  else if (o->type() == "real") {

  }
  else if (o->type() == "record") {

  }
  else {

  }
}

void pinMode(int pin, int mode) {

}

void digitalWrite(int pin, int value) {
  Arduino::digitalWrite(pin, )
}

char Serial.read() {

}


void delay(int ms) {
  Arduino::delay(ms);
}



#endif
