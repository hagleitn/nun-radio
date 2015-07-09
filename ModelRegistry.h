#ifndef ModelRegistry_h
#define ModelRegistry_h

#include "Arduino.h"

// reta
class ModelRegistry {
 private:
  int current_;
  int num = 3;

  float elevon[2][4] = {{0,0.5,0,0.5},{0,-0.5,0,0.5}};
  float full[4][4] = {{0,0,0,1},{0,0,0,1},{0,1,0,0},{1,0,0,0}};
  float input[4] = {};
  float channels[4] = {};
  float trim[4] = {};
  float expo[4] = {0.3,0.3,0.3,0.3};
  float drLow[4] = {0.8,0.8,0.8,0.8};
  float drHigh[4] = {1,1,1,1};

  Model models[3] =
    {
      {
        "bee",
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

  void begin() {
    current_ = 0;
  }

  Model *current() {
    return &models[current_];
  }

  Model *next() {
    if(++current_ >= num) {
      current_ = 0;
    }
    return &models[current_];
  }

  Model *previous() {
    if (--current_ < 0) {
      current_ = num - 1;
    }
    return &models[current_];
  }
};

#endif
