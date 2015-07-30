#include "Voltmeter.h"

Voltmeter::Voltmeter(uint8_t pin, long r1, long r2) : vin(0), pin(pin), r1(r1), r2(r2) {}

void Voltmeter::begin() {
  Serial.begin(9600);
  pinMode(pin, INPUT);
}

void Voltmeter::update() {
  long value = analogRead(pin);
  // float vout = (value * 5) / 1024.0;
  // float vin = vout /  (r2 / ((float)(r1 + r2)));
  // if (vin<0.09) {
  //   vin=0.0;
  // }
  // this->vin = VOLTS_TO_BYTE(vin);
  this->vin = COMPUTE_VOLTS(value, r1, r2);
}
