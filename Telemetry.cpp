#include "Telemetry.h"

#define MAX_ITEMS 20

Telemetry::Telemetry()
  : current(0), a1(0), a2(0), rssi(0), hasTelemetry(false)
#ifdef SOFTWARE_TELEMETRY
  ,in(RXPIN,TXPIN,true)
#endif
{}

void Telemetry::begin() {
  pinMode(RXPIN,INPUT);
  pinMode(TXPIN,OUTPUT);
  SERIAL_STREAM.begin(9600);
}

void Telemetry::update() {
  hasTelemetry = false;
  uint8_t i = 0;

  while ((SERIAL_STREAM.available() > 0) && (i++ < MAX_ITEMS)) {
    buffer[current] = SERIAL_STREAM.read();

#ifdef DEBUG
    Serial.println(buffer[current], HEX);
#endif

    current = (current+1) % 5;

    if (buffer[current] == 0x7E && buffer[(current+1)%5] == 0xFE) {
      hasTelemetry = true;
      // telemetry header:
      // next bytes: a1 a2 rssi
      a1 = buffer[(current+2)%5];
      a2 = buffer[(current+3)%5];
      rssi = buffer[(current+4)%5];
      break;
    }
  }
}
