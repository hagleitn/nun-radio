#ifndef Model_h
#define Model_h

#include "Arduino.h"

typedef struct model_ {
  char const *name;
  float *expo;
#ifdef ENABLE_DUAL_RATES
  float *drLow;
  float *drHigh;
#endif
#ifdef ENABLE_ALARM
  uint8_t *minVolts;
  uint8_t *minSignals;
#endif
  float *transform;
  float *channels;
  float *inputs;
  float *trim;
  uint8_t numChannels;
  uint8_t numInputs;
  uint8_t id;
} Model;

#endif
