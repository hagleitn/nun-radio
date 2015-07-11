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

  float inputs[4] = {};

 private:
  void normalize(float *x, uint8_t n);
  void setInputs();
  void handleButtons(unsigned long currentTime);

 public:

  inline Controller();
  inline void begin();
  inline void update(unsigned long currentTime);
  inline float *getInputs();
  inline bool cPressed();
  inline bool zPressed();
  inline void setMode(bool mode2);
  inline bool getMode();
  inline bool bothPressed();
};

inline Controller::Controller() :
		  zPressed_(false), cPressed_(false),
		  bothPressed_(false), lastZ(0),
		  lastC(0), mode2(true) {}

inline void Controller::begin() {
  chuck.begin();
  chuck.update();
}

inline void Controller::update(unsigned long currentTime) {
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
