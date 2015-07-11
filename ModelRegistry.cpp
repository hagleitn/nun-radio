#include "ModelRegistry.h"

void ModelRegistry::begin() {
  current_ = 0;
}

Model *ModelRegistry::current() {
  return &models[current_];
}

Model *ModelRegistry::next() {
  if(++current_ >= NUM_MODELS) {
    current_ = 0;
  }
  return &models[current_];
}

Model *ModelRegistry::previous() {
  if (--current_ >= NUM_MODELS) {
    current_ = NUM_MODELS - 1;
  }
  return &models[current_];
}
