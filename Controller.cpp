#include "Controller.h"

void Controller::normalize(float *x, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    if (x[i] > 1) x[i] = 1;
    if (x[i] < -1) x[i] = -1;
  }
}

void Controller::setInputs() {
  chuck.update();
  if (mode2) {
    inputs[0] = chuck.readJoyX()/(MAX_JOY/2.0);
    inputs[1] = -(chuck.readPitch()/90.0 - 1) * 2;
    inputs[2] = chuck.readJoyY()/(MAX_JOY/2.0);
    inputs[3] = chuck.readRoll()/90.0 * 2;
  } else {
    inputs[3] = chuck.readJoyX()/(MAX_JOY/2.0);
    inputs[2] = -(chuck.readPitch()/90.0 - 1) * 2;
    inputs[1] = chuck.readJoyY()/(MAX_JOY/2.0);
    inputs[0] = chuck.readRoll()/90.0 * 2;
  }
  normalize(inputs, 4);
}


void Controller::handleButtons(unsigned long currentTime) {

  if (chuck.zPressed()) {
    lastZ = currentTime;
  }

  if (chuck.cPressed()) {
    lastC = currentTime;
  }

  if (currentTime - lastZ > 100
      && currentTime - lastZ < 200
      && currentTime - lastC > 200) {
    lastZ = -1000;
    zPressed_ = true;
  }

  if (currentTime - lastC > 100
      && currentTime - lastC < 200
      && currentTime - lastZ > 200) {
    lastC = -1000;
    cPressed_ = true;
  }

  if (currentTime - lastC > 100
      && currentTime - lastC < 200
      && currentTime - lastZ > 100
      && currentTime - lastZ < 200) {
    lastC = -1000;
    lastZ = -1000;
    bothPressed_ = true;
  }
}
