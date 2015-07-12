#ifndef Alarm_h
#define Alarm_h

#include "Arduino.h"
#include "Battery.h"

#define ALARM_FREQ (440)
#define ALARM_BEEPS (2)
#define ALARM_DURATION (200)
#define ALARM_SEPARATION (200)
#define ALARM_RESET (3000)

#define SIGNAL_FREQ (880)
#define SIGNAL_BEEPS (3)
#define SIGNAL_DURATION (200)
#define SIGNAL_SEPARATION (400)
#define SIGNAL_RESET (3000)

class Alarm {

 private:

  uint8_t *minVolts;
  uint8_t *minSignals;

  unsigned long last;
  int8_t count;
  uint8_t pin;

  bool voltAlarm;
  bool signalAlarm;

  void playLowVolt(unsigned long currentTime);
  void playLowSignal(unsigned long currentTime);
  void playSequence(unsigned long currentTime, uint16_t freq, uint8_t beeps,
		    uint16_t duration, uint16_t separation, uint16_t reset);

  void ok();

 public:

  Alarm(uint8_t pin);

  inline void setMinVolts(uint8_t *min);
  inline void setMinSignals(uint8_t *min);

  void begin();
  void update(unsigned long currentTime, uint8_t *volts,
	      uint8_t numVolts, uint8_t *signals, uint8_t numSignals);
};

inline void Alarm::setMinVolts(uint8_t *min) {
  minVolts = min;
}

inline void Alarm::setMinSignals(uint8_t *min) {
  minSignals = min;
}
#endif
