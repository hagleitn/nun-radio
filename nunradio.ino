#include <math.h>

#include <Wire.h>
#include "Radio.h"
#include "Voltmeter.h"
#include "LCD.h"
#include "ModelRegistry.h"
#include "Telemetry.h"
#include "Controller.h"

#ifdef ENABLE_ALARM
#include "Alarm.h"
#endif

Voltmeter vmeter(A1, 100000l, 30000l);
Telemetry telemetry;

#ifdef ENABLE_ALARM
Alarm alarm(6);
#endif

Controller controller;

Radio radio(5);

LCD lcd;
ModelRegistry registry;

unsigned long lastM = 0;
unsigned long currentTime = 0;
unsigned int counter = 0;

uint8_t volts[3] = {};
uint8_t signals[1] = {};

void setModel(Model*, float*);
void handleButtons(float *);

void setup() {
  lcd.begin();

  radio.begin();

  controller.begin();

#ifdef ENABLE_ALARM
  alarm.begin();
#endif

  registry.begin();
  setModel(registry.current(), controller.getInputs());

  vmeter.begin();

  telemetry.begin();
}

void loop() {
  delay(50);
  currentTime = millis();
  ++counter;

  if (counter % 20 == 0) {
    vmeter.update();
    volts[0] = vmeter.getVoltage();

    telemetry.update();

    volts[1] = telemetry.getA1();
    volts[2] = telemetry.getA2();
    signals[0] = telemetry.getRssi();

    lcd.setTelemetryAvailable(telemetry.aquired());
    lcd.setVolts(volts[0]);
    lcd.setRssi(signals[0]);
    lcd.setA1(volts[1]);
    lcd.setA2(volts[2]);
  }

#ifdef ENABLE_ALARM
  if (counter % 8 == 0) {
    alarm.update(currentTime, volts,
		 telemetry.aquired() ? 3 : 1, signals,
		 telemetry.aquired() ? 1 : 0);
  }
#endif


  controller.update(currentTime);

  handleButtons(controller.getInputs());

  radio.update(controller.getInputs());

  //  if (counter % 10 == 0) {
  lcd.update();
  //  }
}

void handleButtons(float *inputs) {
  if (controller.bothPressed()) {
    if (currentTime - lastM > 500) {
      if (inputs[controller.getMode() ? 2 : 1] < -0.7) {
        lastM = currentTime;
        setModel(registry.next(), inputs);
      } else if (inputs[controller.getMode() ? 2 : 1] > 0.7) {
        lastM = currentTime;
        setModel(registry.previous(), inputs);
      }
    }
  }

  if (controller.zPressed()) {
    controller.setMode(!controller.getMode());
  }

  if (controller.cPressed()) {
    radio.setTrim(inputs, registry.current()->numInputs);
  }
}

void setModel(Model *m, float *inputs) {
  radio.setModel(m);

  lcd.setChannels(radio.getChannels(), m->numChannels);
  lcd.setModelName(m->name);
  lcd.setRssi(0);
  lcd.setInputs(inputs, m->numInputs);

#ifdef ENABLE_ALARM
  alarm.setMinVolts(m->minVolts);
  alarm.setMinSignals(m->minSignals);
#endif
}
