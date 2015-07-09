#ifndef Radio_h
#define Radio_h

#include "Arduino.h"
#include "RCEncoder.h"
#include <math.h>

#define MIN_SERVO_PULSE 544
#define MAX_SERVO_PULSE 2400
#define RANGE (MAX_SERVO_PULSE - MIN_SERVO_PULSE)

typedef struct model_ {
  char *name;
  float *expo;
  float *drLow;
  float *drHigh;
  float *transform;
  int numChannels;
  float *channels;
  int numInputs;
  float *inputs;
  float *trim;
} Model;

class Radio {

 private:
  Model *model;
  float *inputs;
  float *channels;
  float *trim;
  bool lowRates;
  int pin;

 public:

  Radio(int pin) {
    this->pin = pin;
  }

  void begin() {
    pinMode(pin, OUTPUT);
    encoderBegin(pin);
  }

  void setModel(Model *m) {
    this->model = m;

    this->channels = m->channels;
    this->inputs = m->inputs;
    this->trim = m->trim;

    init(this->inputs, this->model->numInputs, 0);
    init(this->channels, this->model->numChannels, 0);

    init(this->trim, this->model->numInputs, 0);

    this->lowRates = false;
  }

  void send(float *inputs) {

    int n = this->model->numInputs;
    int m = this->model->numChannels;

    // apply trim
    subtract(this->inputs, inputs, this->trim, n);

    // appy expo to input
    for (int i = 0; i < n; ++i) {
      this->inputs[i] = expo(this->model->expo[i], this->inputs[i]);
    }

    // process the mix
    mix(this->model->transform, this->inputs, this->channels, n, m);

    // limit throw with (dual) rates
    mult(this->channels, this->channels, this->lowRates ? this->model->drLow : this->model->drHigh, m);

    // ppm encode and send out
    for (int i = 0; i < m; ++i) {
      encoderWrite(i, toServo(&(this->channels[i])));
    }
  }

  float *getChannels() {
    return this->channels;
  }

  float *getTrim() {
    return this->trim;
  }

  void setTrim(float *trim, int n) {
    for (int i = 0; i < n; ++i) {
      this->trim[i] = trim[i];
    }
  }

  void setLowRates() {
    this->lowRates = true;
  }

  void setHighRates() {
    this->lowRates = false;
  }

 private:

  float toServo(float *input) {

    if (*input > 1) {
      *input = 1;
    } else if (*input < -1) {
      *input = -1;
    }

    float val = MIN_SERVO_PULSE + (1 + *input) * RANGE / 2.0;
    return val;
  }

  // y = a*x^3+(1-a)x, a .. expo (0 >= a >= 1), x .. input (-1 <= x <= 1)
  float expo(float a, float x) {
    return a*x*x*x+(1-a)*x;
  }

  /**
   * mix is matrix multiplication:  channel = transform * inputs
   * transform nxm matrix
   * inputs n float values coming from the controler
   * channels m outputs
   */
  void mix(float *transform, float *inputs, float *channels, int n, int m) {
    init(channels, m, 0);
    for (int j = 0; j < m; ++j)  {
      for (int i = 0; i < n; ++i) {
        channels[j] = channels[j] + inputs[i] * transform[i+j*n];
      }
    }
  }

  void init(float *x, int n, float c) {
    for (int i = 0; i < n; ++i) {
      x[i] = c;
    }
  }

  void add(float *y, float *x1, float *x2, int n) {
    for (int i = 0; i < n; ++i) {
      y[i] = x1[i] + x2[i];
    }
  }

  void scale(float *y, float *x, int scale, int n) {
    for (int i = 0; i < n; ++i) {
      y[i] = scale * x[i];
    }
  }

  void mult(float *y, float *x1, float *x2, int n) {
    for (int i = 0; i < n; ++i) {
      y[i] = x1[i] * x2[i];
    }
  }

  void subtract(float *y, float *x1, float *x2, int n) {
    for (int i = 0; i < n; ++i) {
      y[i] = x1[i] - x2[i];
    }
  }
};

#endif
