#include "Radio.h"

Radio::Radio(uint8_t pin) : pin(pin)
#ifdef ENABLE_DUAL_RATES
			  , lowRates(false)
#endif
{}

void Radio::begin() {
  pinMode(pin, OUTPUT);
  encoderBegin(pin);
}

void Radio::setModel(Model *m) {
  this->model = m;

  this->channels = m->channels;
  this->inputs = m->inputs;
  this->trim = m->trim;

  init(this->inputs, this->model->numInputs, 0);
  init(this->channels, this->model->numChannels, 0);

  init(this->trim, this->model->numInputs, 0);

#ifdef ENABLE_DUAL_RATES
  this->lowRates = false;
#endif
}

void Radio::update(float *inputs) {

  uint8_t n = this->model->numInputs;
  uint8_t m = this->model->numChannels;

  // apply trim
  add(this->inputs, inputs, this->trim, n);

  // appy expo to input
  for (uint8_t i = 0; i < n; ++i) {
    this->inputs[i] = expo(this->model->expo[i], this->inputs[i]);
  }

  // process the mix
  mix(this->model->transform, this->inputs, this->channels, n, m);

#ifdef ENABLE_DUAL_RATES
  // limit throw with (dual) rates
  mult(this->channels, this->channels, this->lowRates ? this->model->drLow : this->model->drHigh, m);
#endif

  // ppm encode and send out
  for (uint8_t i = 0; i < m; ++i) {
    encoderWrite(i, toServo(&(this->channels[i])));
  }
}

float *Radio::getChannels() {
  return this->channels;
}

float *Radio::getTrim() {
  return this->trim;
}

void Radio::setTrim(float *trim, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    this->trim[i] += trim[i];
  }
}

#ifdef ENABLE_DUAL_RATES
void Radio::setLowRates() {
  this->lowRates = true;
}

void Radio::setHighRates() {
  this->lowRates = false;
}
#endif

float Radio::toServo(float *input) {

  if (*input > 1) {
    *input = 1;
  } else if (*input < -1) {
    *input = -1;
  }

  float val = MIN_SERVO_PULSE + (1 + *input) * RANGE / 2.0;
  return val;
}

/**
 * mix is matrix multiplication:  channel = transform * inputs
 * transform nxm matrix
 * inputs n float values coming from the controler
 * channels m outputs
 */
void Radio::mix(float *transform, float *inputs, float *channels, uint8_t n, uint8_t m) {
  init(channels, m, 0);
  for (uint8_t j = 0; j < m; ++j)  {
    for (uint8_t i = 0; i < n; ++i) {
      channels[j] = channels[j] + inputs[i] * transform[i+j*n];
    }
  }
}

