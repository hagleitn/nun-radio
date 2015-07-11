#include "Telemetry.h"

Telemetry::Telemetry() : in(RXPIN,TXPIN,true) {};

void Telemetry::begin() {
  pinMode(RXPIN,INPUT);
  pinMode(TXPIN,OUTPUT);
  in.begin(9600);
}

void Telemetry::update() {
  while (in.available() > 0) {
    buffer[current] = in.read();
    current = (current+1) % 5;

    if (buffer[current] == 0x7E && buffer[(current+1)%5] == 0xFE) {
      // telemetry header:
      // next bytes: a1 a2 rssi
      a1 = buffer[(current+2)%5];
      a2 = buffer[(current+3)%5];
      rssi = buffer[(current+4)%5];
      break;
    }
  }
}
