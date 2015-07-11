#include "Voltmeter.h"

Voltmeter::Voltmeter(uint8_t pin, float r1, float r2) : pin(pin), r1(r1), r2(r2), vin(vin) {}

void Voltmeter::begin() {
  pinMode(pin, INPUT);
}

void Voltmeter::update() {
  float value = analogRead(pin);
  float vout = (value * 5.0) / 1024.0;
  this->vin = vout /  (r2 / (r1 + r2));
  if (this->vin<0.09) {
    this->vin=0.0;
  }
}
