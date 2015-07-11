#ifndef ModelRegistry_h
#define ModelRegistry_h

#include "Arduino.h"
#include "Radio.h"

#define NUM_MODELS 3

// reta
class ModelRegistry {
 private:
  uint8_t current_;

  float elevon[2][4] = {{0,0.5,0,0.5},{0,-0.5,0,0.5}};
  float full[4][4] = {{0,0,0,1},{0,0,0,1},{0,1,0,0},{1,0,0,0}};
  float input[4] = {};
  float channels[4] = {};
  float trim[4] = {};
  float expo[4] = {0.3,0.3,0.3,0.3};
  float drLow[4] = {0.8,0.8,0.8,0.8};
  float drHigh[4] = {1,1,1,1};

  Model models[NUM_MODELS] =
    {
      {
        "bee2",
        expo,
        drLow,
        drHigh,
        (float*)elevon,
        2,
        channels,
        4,
        input,
        trim
      },
      {
        "raptor",
        expo,
        drLow,
        drHigh,
        (float*)elevon,
        2,
        channels,
        4,
        input,
        trim
      },
      {
        "libelle",
        expo,
        drLow,
        drHigh,
        (float*)full,
        4,
        channels,
        4,
        input,
        trim
      }
    };

 public:

  void begin();
  Model *current();
  Model *next();
  Model *previous();
};

#endif
