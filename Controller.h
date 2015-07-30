#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "WiiChuck.h"

class Controller {

 private:
  WiiChuck chuck;
  bool zPressed_, cPressed_, bothPressed_;

  unsigned long lastZ;
  unsigned long lastC;

  bool mode2 = true;

  int minPitch;
  int maxPitch;
  int minRoll;
  int maxRoll;
  int minX;
  int maxX;
  int minY;
  int maxY;
  int zeroPitch;
  int zeroRoll;
  int zeroX;
  int zeroY;

  int16_t inputs[4] = {};

 private:
  int16_t normalize(int x, int zero, int min, int max);
  void setInputs();
  void handleButtons(unsigned long currentTime);

 public:

  inline Controller();
  inline void begin();
  inline void update(unsigned long currentTime);
  inline int16_t *getInputs();
  inline bool cPressed();
  inline bool zPressed();
  inline void setMode(bool mode2);
  inline bool getMode();
  inline bool bothPressed();
#ifdef ENABLE_CALIBRATION
  void calibrateZero();
  void calibrateMax();
#endif
};

inline Controller::Controller() :
                  zPressed_(false), cPressed_(false),
                  bothPressed_(false), lastZ(0),
                  lastC(0), mode2(true), minPitch(45),
                  maxPitch(135), minRoll(-45), maxRoll(45),
                  minX(-127), maxX(128), minY(-127), maxY(128),
                  zeroPitch(90), zeroRoll(0), zeroX(0), zeroY(0)
{}

inline void Controller::begin() {
  chuck.begin();
  chuck.update();
}

inline void Controller::update(unsigned long currentTime) {
  setInputs();
  handleButtons(currentTime);
}

inline int16_t *Controller::getInputs() {
  return inputs;
}

inline bool Controller::cPressed() {
  bool res = cPressed_;
  cPressed_ = false;
  return res;
}

inline bool Controller::zPressed() {
  bool res = zPressed_;
  zPressed_ = false;
  return res;
}

inline void Controller::setMode(bool mode2) {
  this->mode2 = mode2;
}

inline bool Controller::getMode() {
  return mode2;
}

inline bool Controller::bothPressed() {
  bool res = bothPressed_;
  bothPressed_ = false;
  return res;
}

#endif
