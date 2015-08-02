#ifndef Model_h
#define Model_h

#include "Arduino.h"

#define DIVISOR_PERCENTS (32)
#define PERCENT_TO_BYTE(P) ((int8_t)((((int)P) * DIVISOR_PERCENTS) / 100))
#define BYTE_TO_PERCENT(P) ((int8_t)((((int)P) * 100) / DIVISOR_PERCENTS))

#define MAX_LEVEL 512
#define MIN_LEVEL -512

#define MULT(A, B) ((A) * (B) / DIVISOR_PERCENTS)

typedef struct model_ {
  char const *name;
  int8_t *expo;
#ifdef ENABLE_DUAL_RATES
  int8_t *drLow;
  int8_t *drHigh;
#endif
#ifdef ENABLE_ALARM
  uint8_t *minVolts;
  uint8_t *minSignals;
#endif
  int8_t *transform;
  int *channels;
  int *inputs;
  int *trim;
  uint8_t numChannels;
  uint8_t numInputs;
  uint8_t id;
} Model;

#endif
