#ifndef Radio_h
#define Radio_h

#include "Arduino.h"
#include "RCEncoder.h"
#include "Model.h"

#define MIN_SERVO_PULSE 800
#define MAX_SERVO_PULSE 1800
#define RANGE (MAX_SERVO_PULSE - MIN_SERVO_PULSE)

class Radio {

 private:
  Model *model;
  int16_t *inputs;
  int16_t *channels;
  int16_t *trim;
#ifdef ENABLE_DUAL_RATES
  bool lowRates;
  uint8_t buzzPin;
#endif
  uint8_t radioPin;

 public:

  Radio(uint8_t radioPin
#ifdef ENABLE_DUAL_RATES
	, uint8_t buzzPin
#endif
	);
  void begin();
  void setModel(Model *m);
  void update(int *inputs);
  int16_t *getInputs();
  int16_t *getChannels();
  int16_t *getTrim();
  void setTrim(int16_t *inputs, uint8_t n);

#ifdef ENABLE_DUAL_RATES
  void toggleRates();
  bool isLowRates();
#endif

 private:

  int16_t toServo(int16_t *input);
  inline int16_t norm(int16_t val);
  inline int16_t expo(int8_t a, int32_t x);
  void mix(int8_t *transform, int16_t *inputs, int16_t *channels, uint8_t n, uint8_t m);
  inline void init(int16_t *x, uint8_t n, int16_t c);
  inline void add(int16_t *y, int16_t *x1, int16_t *x2, uint8_t n);
  inline void scale(int16_t *y, int16_t *x, uint8_t scale, uint8_t n);
  inline void mult(int16_t *y, int16_t *x1, int8_t *x2, uint8_t n);
};

inline int16_t Radio::norm(int16_t v) {
  if (v > MAX_LEVEL) return MAX_LEVEL;
  if (v < MIN_LEVEL) return MIN_LEVEL;
  return v;
}

// y = a*x^3+(1-a)x, a .. expo (0 >= a >= 1), x .. input (-1 <= x <= 1)
inline int16_t Radio::expo(int8_t a, int32_t x) {
  return (((BYTE_TO_PERCENT(a) * x  * x) / 100) * x) / (((int32_t)MAX_LEVEL) * MAX_LEVEL)
    + (100 - BYTE_TO_PERCENT(a)) * x / 100;
}

inline void Radio::init(int16_t *x, uint8_t n, int16_t c) {
  for (uint8_t i = 0; i < n; ++i) {
    x[i] = c;
  }
}

inline void Radio::add(int16_t *y, int16_t *x1, int16_t *x2, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    y[i] = norm(x1[i] + x2[i]);
  }
}

inline void Radio::scale(int16_t *y, int16_t *x, uint8_t scale, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    y[i] = scale * x[i];
  }
}

inline void Radio::mult(int16_t *y, int16_t *x1, int8_t *x2, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    y[i] = MULT(x1[i], x2[i]);
  }
}
#endif
