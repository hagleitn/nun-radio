#ifndef Voltmeter_h
#define Voltmeter_h

#include "Arduino.h"

class Voltmeter {

 private:
  float vin;
  int pin;
  float r1;
  float r2;

 public:

  Voltmeter(int pin, float r1, float r2) {
    this->r1 = r1;
    this->r2 = r2;
    this->pin = pin;
    this->vin = 0;
  }

  void update() {
    float value = analogRead(pin);
    float vout = (value * 5.0) / 1024.0;
    this->vin = vout /  (r2 / (r1 + r2));
    if (this->vin<0.09) {
      this->vin=0.0;
    }
  }

  float getVoltage() {
    return this->vin;
  }
};

#endif
