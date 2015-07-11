#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "WiiChuck.h"

class Controller {

 private:
  WiiChuck chuck;
  bool zPressed_, cPressed_, bothPressed_;

  long lastZ = -1000;
  long lastC = -1000;

  bool mode2 = true;

  float inputs[4];

 private:
  void normalize(float *x, int n);
  void setInputs();
  void handleButtons(long currentTime);

 public:

  inline void begin();
  inline void update(long currentTime);
  inline float *getInputs();
  inline bool cPressed();
  inline bool zPressed();
  inline void setMode(bool mode2);
  inline bool getMode();
  inline bool bothPressed();
};

inline void Controller::begin() {
  chuck.begin();
  chuck.update();
}

inline void Controller::update(long currentTime) {
  setInputs();
  handleButtons(currentTime);
}

inline float *Controller::getInputs() {
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
