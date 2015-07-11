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

  Radio(int pin);
  void begin();
  void setModel(Model *m);
  void update(float *inputs);
  float *getChannels();
  float *getTrim();
  void setTrim(float *trim, int n);
  void setLowRates();
  void setHighRates();

 private:

  float toServo(float *input);
  inline float expo(float a, float x);
  void mix(float *transform, float *inputs, float *channels, int n, int m);
  inline void init(float *x, int n, float c);
  inline void add(float *y, float *x1, float *x2, int n);
  inline void scale(float *y, float *x, int scale, int n);
  inline void mult(float *y, float *x1, float *x2, int n);
};

// y = a*x^3+(1-a)x, a .. expo (0 >= a >= 1), x .. input (-1 <= x <= 1)
inline float Radio::expo(float a, float x) {
  return a*x*x*x+(1-a)*x;
}

inline void Radio::init(float *x, int n, float c) {
  for (int i = 0; i < n; ++i) {
    x[i] = c;
  }
}

inline void Radio::add(float *y, float *x1, float *x2, int n) {
  for (int i = 0; i < n; ++i) {
    y[i] = x1[i] + x2[i];
  }
}

inline void Radio::scale(float *y, float *x, int scale, int n) {
  for (int i = 0; i < n; ++i) {
    y[i] = scale * x[i];
  }
}

inline void Radio::mult(float *y, float *x1, float *x2, int n) {
  for (int i = 0; i < n; ++i) {
    y[i] = x1[i] * x2[i];
  }
}
#endif
