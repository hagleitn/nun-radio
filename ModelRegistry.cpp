#include "ModelRegistry.h"

void ModelRegistry::begin() {
  current_ = 0;
}

Model *ModelRegistry::current() {
  return &models[current_];
}

Model *ModelRegistry::next() {
  if(++current_ >= num) {
    current_ = 0;
  }
  return &models[current_];
}

Model *ModelRegistry::previous() {
  if (--current_ < 0) {
    current_ = num - 1;
  }
  return &models[current_];
}
