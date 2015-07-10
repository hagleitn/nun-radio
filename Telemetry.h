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

 Telemetry() : in(RXPIN,TXPIN,true) {};

  void begin() {
    //Serial1.begin(9600, SERIAL_8N1);
    pinMode(RXPIN,INPUT);
    pinMode(TXPIN,OUTPUT);
    Serial.begin(9600);
    in.begin(9600);
  }

  void update() {
    while (in.available() > 0) {
      buffer[current] = in.read();
      //      Serial.print("Received: ");
      Serial.println(buffer[current],HEX);
      current = (current+1) % 5;

      if (buffer[current] == 0x7E && buffer[(current+1)%5] == 0xFE) {
        // telemetry header:
        // next bytes: a1 a2 rssi
        a1 = buffer[(current+2)%5];
        a2 = buffer[(current+3)%5];
        rssi = buffer[(current+4)%5];
        //      Serial.print("rssi: ");
        //      Serial.println(rssi);
        break;
      }
    }
  }

  int getRssi() {
    return rssi;
  }

  int getA1() {
    return a1;
  }

  int getA2() {
    return a2;
  }
};

#endif
