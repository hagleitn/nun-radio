#ifndef Telemetry_h
#define Telemetry_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#define RXPIN 10
#define TXPIN 11

class Telemetry {

 private:
  uint8_t buffer[5] = {};
  int current = 0;
  int a1;
  int a2;
  int rssi;
  SoftwareSerial in;

 public:

  Telemetry();

  void begin();
  void update();
  inline int getRssi();
  inline int getA1();
  inline int getA2();
};

inline int Telemetry::getRssi() {
  return rssi;
}

inline int Telemetry::getA1() {
  return a1;
}

inline int Telemetry::getA2() {
  return a2;
}

#endif
