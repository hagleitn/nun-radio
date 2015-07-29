#include "Controller.h"

float Controller::normalize(int x, int zero, int min, int max) {
  if (min == max) return min;

  float val = (x - zero) / ((max - min)/2.0);
  if (val > 1) val = 1;
  if (val < -1) val = -1;
  return val;
}

void Controller::setInputs() {
  chuck.update();

  inputs[0] = normalize(chuck.readJoyX(), zeroX, minX, maxX);
  inputs[1] = -normalize(chuck.readPitch(), zeroPitch, minPitch, maxPitch);
  inputs[2] = normalize(chuck.readJoyY(), zeroY, minY, maxY);
  inputs[3] = normalize(chuck.readRoll(), zeroRoll, minRoll, maxRoll);

  if (!mode2) {
    float t = inputs[3];

    inputs[3] = inputs[0];
    inputs[0] = t;

    t = inputs[2];
    inputs[2] = inputs[1];
    inputs[1] = t;
  }
}

#ifdef ENABLE_CALIBRATION
void Controller::calibrate() {
  do {
    delay(10);
    chuck.update();
    zeroX = chuck.readJoyX();
    zeroY = chuck.readJoyY();
    zeroPitch = chuck.readPitch();
    zeroRoll = chuck.readRoll();
  } while (!chuck.cPressed());

  do {
    delay(10);
    chuck.update();
    minPitch = min(chuck.readPitch(), minPitch);
    maxPitch = max(chuck.readPitch(), maxPitch);
    minRoll = min(chuck.readRoll(), minRoll);
    maxRoll = max(chuck.readRoll(), maxRoll);
    minX = min(chuck.readJoyX(), minX);
    maxX = max(chuck.readJoyX(), maxX);
    minY = min(chuck.readJoyY(), minY);
    maxY = max(chuck.readJoyY(), maxY);
  } while (!chuck.cPressed());
}
#endif

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
