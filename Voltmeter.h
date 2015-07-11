#ifndef Voltmeter_h
#define Voltmeter_h

#include "Arduino.h"

class Voltmeter {

 private:
  float vin;
  uint8_t pin;
  float r1;
  float r2;

 public:

  Voltmeter(uint8_t pin, float r1, float r2);
  void begin();
  void update();
  inline float getVoltage();
};

inline float Voltmeter::getVoltage() {
  return this->vin;
}

#endif
