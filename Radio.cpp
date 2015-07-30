#include "Radio.h"

#define LOW_FREQ (220)
#define HIGH_FREQ (880)
#define BUZZ_DURATION (500)

Radio::Radio(uint8_t radioPin
#ifdef ENABLE_DUAL_RATES
	     , uint8_t buzzPin
#endif
	     ) : radioPin(radioPin)
#ifdef ENABLE_DUAL_RATES
             , buzzPin(buzzPin)
             , lowRates(false)
#endif
{}

void Radio::begin() {
  pinMode(radioPin, OUTPUT);
  encoderBegin(radioPin);
}

void Radio::setModel(Model *m) {
  this->model = m;

  this->channels = m->channels;
  this->inputs = m->inputs;
  this->trim = m->trim;

  init(this->inputs, this->model->numInputs, 0);
  init(this->channels, this->model->numChannels, 0);

#ifdef ENABLE_DUAL_RATES
  this->lowRates = false;
#endif
}

void Radio::update(int16_t *inputs) {

  uint8_t n = this->model->numInputs;
  uint8_t m = this->model->numChannels;

  // appy expo to input
  for (uint8_t i = 0; i < n; ++i) {
    this->inputs[i] = expo(this->model->expo[i], inputs[i]);
  }

  // apply trim
  add(this->inputs, this->inputs, this->trim, n);

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

int16_t *Radio::getChannels() {
  return this->channels;
}

int16_t *Radio::getInputs() {
  return this->inputs;
}

int16_t *Radio::getTrim() {
  return this->trim;
}

void Radio::setTrim(int16_t *inputs, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    this->trim[i] += expo(this->model->expo[i], inputs[i]);
  }
}

#ifdef ENABLE_DUAL_RATES
void Radio::toggleRates() {
  this->lowRates = !this->lowRates;
  tone(buzzPin, lowRates ? LOW_FREQ : HIGH_FREQ, BUZZ_DURATION);
}

bool Radio::isLowRates() {
  return this->lowRates;
}
#endif

int16_t Radio::toServo(int16_t *input) {
  return MIN_SERVO_PULSE + (MAX_LEVEL + norm(*input)) - 12;
}

/**
 * mix is matrix multiplication:  channel = transform * inputs
 * transform nxm matrix
 * inputs n int16_t values coming from the controler
 * channels m outputs
 */
void Radio::mix(int8_t *transform, int16_t *inputs, int16_t *channels, uint8_t n, uint8_t m) {
  init(channels, m, 0);
  for (uint8_t j = 0; j < m; ++j)  {
    for (uint8_t i = 0; i < n; ++i) {
      channels[j] = norm(channels[j] + MULT(inputs[i], transform[i+j*n]));
    }
  }
}
