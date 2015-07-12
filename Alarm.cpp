#include "Alarm.h"

Alarm::Alarm(uint8_t pin) : minVolts(0), minSignals(0), last(0),
                            count(0), pin(pin), voltAlarm(false), signalAlarm(false) {}

void Alarm::update(unsigned long currentTime, uint8_t *volts, uint8_t numVolts,
                   uint8_t *signals, uint8_t numSignals) {
  bool enteredAlarm = false;

  for (uint8_t i = 0; i < numSignals; ++i) {
    if (minSignals[i] != 0xFF && signals[i] < minSignals[i]) {
      signalAlarm = true;
      enteredAlarm = true;
    }
  }

  for (uint8_t i = 0; i < numVolts; ++i) {
    if (minVolts[i] != 0xFF && volts[i] < minVolts[i]) {
      voltAlarm = true;
      enteredAlarm = true;
    }
  }

  if (signalAlarm) {
    playLowSignal(currentTime);
  } else if (voltAlarm) {
    playLowVolt(currentTime);
  }

  if (!enteredAlarm && count == 0) {
    ok();
  }
}

void Alarm::ok() {
  voltAlarm = false;
  signalAlarm = false;
  last = 0;
  count = 0;
}

void Alarm::begin() {
  pinMode(pin, OUTPUT);
}

void Alarm::playLowVolt(unsigned long currentTime) {
  playSequence(currentTime, ALARM_FREQ, ALARM_BEEPS, ALARM_DURATION,
               ALARM_SEPARATION, ALARM_RESET);
}

void Alarm::playLowSignal(unsigned long currentTime) {
  playSequence(currentTime, SIGNAL_FREQ, SIGNAL_BEEPS,
               SIGNAL_DURATION, SIGNAL_SEPARATION, SIGNAL_RESET);
}

void Alarm::playSequence(unsigned long currentTime, uint16_t freq, uint8_t beeps,
                         uint16_t duration, uint16_t separation, uint16_t reset) {
  if (count == 0) {
    ++count;
    tone(pin, freq, duration);
    last = currentTime;
  } else if (count < beeps) {
    if (currentTime - last > separation) {
      ++count;
      tone(pin, freq, duration);
      last = currentTime;
    }
  } else {
    if (currentTime - last > reset) {
      count = 0;
      last = 0;
    }
  }
}
