#include "Voltmeter.h"

Voltmeter::Voltmeter(int pin, float r1, float r2) {
  this->r1 = r1;
  this->r2 = r2;
  this->pin = pin;
  this->vin = 0;
}

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
