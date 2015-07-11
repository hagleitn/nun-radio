#ifndef Radio_h
#define Radio_h

//#define DUAL_RATES

#include "Arduino.h"
#include "RCEncoder.h"
#include <math.h>

#define MIN_SERVO_PULSE 1000
#define MAX_SERVO_PULSE 2000
#define RANGE (MAX_SERVO_PULSE - MIN_SERVO_PULSE)

typedef struct model_ {
  char const *name;
  float *expo;
#ifdef DUAL_RATES
  float *drLow;
  float *drHigh;
#endif
  float *transform;
  float *channels;
  float *inputs;
  float *trim;
  uint8_t numChannels;
  uint8_t numInputs;
} Model;

class Radio {

 private:
  Model *model;
  float *inputs;
  float *channels;
  float *trim;
#ifdef DUAL_RATES
  bool lowRates;
#endif
  uint8_t pin;

 public:

  Radio(uint8_t pin);
  void begin();
  void setModel(Model *m);
  void update(float *inputs);
  float *getChannels();
  float *getTrim();
  void setTrim(float *trim, uint8_t n);

#ifdef DUAL_RATES
  void setLowRates();
  void setHighRates();
#endif

 private:

  float toServo(float *input);
  inline float expo(float a, float x);
  void mix(float *transform, float *inputs, float *channels, uint8_t n, uint8_t m);
  inline void init(float *x, uint8_t n, float c);
  inline void add(float *y, float *x1, float *x2, uint8_t n);
  inline void scale(float *y, float *x, uint8_t scale, uint8_t n);
  inline void mult(float *y, float *x1, float *x2, uint8_t n);
};

// y = a*x^3+(1-a)x, a .. expo (0 >= a >= 1), x .. input (-1 <= x <= 1)
inline float Radio::expo(float a, float x) {
  return a*x*x*x+(1-a)*x;
}

inline void Radio::init(float *x, uint8_t n, float c) {
  for (uint8_t i = 0; i < n; ++i) {
    x[i] = c;
  }
}

inline void Radio::add(float *y, float *x1, float *x2, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    y[i] = x1[i] + x2[i];
  }
}

inline void Radio::scale(float *y, float *x, uint8_t scale, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    y[i] = scale * x[i];
  }
}

inline void Radio::mult(float *y, float *x1, float *x2, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    y[i] = x1[i] * x2[i];
  }
}
#endif
