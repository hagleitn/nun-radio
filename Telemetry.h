#ifndef Telemetry_h
#define Telemetry_h

#include "Arduino.h"
#include "Battery.h"
#include "SoftwareSerial.h"

#define RXPIN 10
#define TXPIN 11

class Telemetry {

 private:
  uint8_t buffer[5] = {};
  uint8_t current;
  uint8_t a1;
  uint8_t a2;
  uint8_t rssi;
  bool hasTelemetry;
  SoftwareSerial in;

 public:

  Telemetry();

  void begin();
  void update();
  inline uint8_t getRssi();
  inline uint8_t getA1();
  inline uint8_t getA2();
  inline bool aquired();
};

inline uint8_t Telemetry::getRssi() {
  return rssi;
}

inline uint8_t Telemetry::getA1() {
  return A1_TO_BYTE(a1);
}

inline uint8_t Telemetry::getA2() {
  return A2_TO_BYTE(a2);
}

inline bool Telemetry::aquired() {
  return hasTelemetry;
}

#endif
