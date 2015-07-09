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

  Model models[3] =
    {
      {
        "bee",
        (float[]){0,0.3,0,0.3},
        (float[]){0.8,0.8},
        (float[]){1,1},
        (float*)elevon,
        2,
        4
      },
      {
        "raptor",
        (float[]){0,0.3,0,0.3},
        (float[]){0.8,0.8},
        (float[]){1,1},
        (float*)elevon,
        2,
        4
      },
      {
        "libelle",
        (float[]){0.3,0.3,0,0.3},
        (float[]){0.8,0.8,0.8,0.8},
        (float[]){1,1,1,1},
        (float*)full,
        4,
        4
      },
    };

 public:

  ModelRegistry() {
    current_ = 0;
  }

  Model *current() {
    return &models[current_];
  }

  Model *next() {
    if(++current_ == num) {
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
