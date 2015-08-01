#include "Controller.h"
#include "Model.h"

int16_t Controller::normalize(int x, int zero, int min, int max) {
  if (min == max) return min;

  int16_t val = (((x - zero) * ((MAX_LEVEL - MIN_LEVEL)/8)) / (max - min)) * 8;
  if (val > MAX_LEVEL) val = MAX_LEVEL;
  if (val < MIN_LEVEL) val = MIN_LEVEL;
  return val;
}

void Controller::setInputs() {
  chuck.update();

  inputs[0] = normalize(chuck.readJoyX(), zeroX, minX, maxX);
  inputs[1] = -normalize(chuck.readPitch(), zeroPitch, minPitch, maxPitch);
  inputs[2] = normalize(chuck.readJoyY(), zeroY, minY, maxY);
  inputs[3] = normalize(chuck.readRoll(), zeroRoll, minRoll, maxRoll);

  if (!mode2) {
    int16_t t = inputs[3];

    inputs[3] = inputs[0];
    inputs[0] = t;

    t = inputs[2];
    inputs[2] = inputs[1];
    inputs[1] = t;
  }
}

#ifdef ENABLE_CALIBRATION
void Controller::calibrateZero() {
  do {
    delay(10);
    chuck.update();
    zeroX = chuck.readJoyX();
    zeroY = chuck.readJoyY();
    zeroPitch = chuck.readPitch();
    zeroRoll = chuck.readRoll();
  } while (!chuck.cPressed());

  minPitch = maxPitch = zeroPitch;
  minRoll = maxRoll = zeroRoll;
  minX = maxX = zeroX;
  minY = maxY = zeroY;
}

void Controller::calibrateMax() {
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
