#ifndef ModelRegistry_h
#define ModelRegistry_h

#include "Arduino.h"
#include "Radio.h"
#include "Battery.h"

#define NUM_MODELS 4

// reta
class ModelRegistry {
 private:

  float elevon[2][4] = {{0,0.5,0,0.5},{0,-0.5,0,0.5}};
  float full[4][4] = {{0,0,0,1},{0,0,0,1},{0,1,0,0},{1,0,0,0}};
  float reta[4][4] = {{1,0,0,0}, {0,-1,0,0}, {0,0,2,0}, {0,0,0,1}};
  float input[4] = {};
  float channels[4] = {};
  float trim[4] = {};
  float expo[4] = {0.4,0.4,0.4,0.4};
#ifdef ENABLE_DUAL_RATES
  float drLow[4] = {0.8,0.8,0.8,0.8};
  float drHigh[4] = {1,1,1,1};
#endif
#ifdef ENABLE_ALARM
  uint8_t minVolts[3] = {VOLTS_TO_BYTE(WARN_VOLTS_LIPO(2)), VOLTS_TO_BYTE(WARN_VOLTS_NIMH(4)), 0xFF};
  uint8_t minSignals[1] = {20};
#endif
  uint8_t current_;

  Model models[NUM_MODELS] =
    {
      {
        "bee2",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
	minVolts,
	minSignals,
#endif
        (float*)elevon,
        channels,
        input,
        trim,
	2,
	4
      },
      {
        "raptor",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
	minVolts,
	minSignals,
#endif
        (float*)elevon,
        channels,
        input,
        trim,
	2,
	4
      },
      {
        "libelle",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
	minVolts,
	minSignals,
#endif
        (float*)full,
        channels,
        input,
        trim,
	4,
	4
      },
      {
        "k8",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
	minVolts,
	minSignals,
#endif
        (float*)reta,
        channels,
        input,
        trim,
	4,
	4
      }
    };

 public:

  void begin();
  Model *current();
  Model *next();
  Model *previous();
};

#endif
