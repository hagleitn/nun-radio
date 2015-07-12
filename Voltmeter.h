#ifndef Voltmeter_h
#define Voltmeter_h

#include "Arduino.h"
#include "Battery.h"

class Voltmeter {

 private:
  uint8_t vin;
  uint8_t pin;
  long r1;
  long r2;

 public:

  Voltmeter(uint8_t pin, long r1, long r2);
  void begin();
  void update();
  inline uint8_t getVoltage();
};

inline uint8_t Voltmeter::getVoltage() {
  return this->vin;
}

#endif
