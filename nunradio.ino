#include <math.h>

#include <Wire.h>
#include "Radio.h"
#include "Voltmeter.h"
#include "LCD.h"
#include "ModelRegistry.h"
#include "Telemetry.h"
#include "Controller.h"

Voltmeter vmeter(A1, 100000.0, 30000.0);
Telemetry telemetry;

Controller controller;

Radio radio(5);

LCD lcd;
ModelRegistry registry;

long lastM = -1000;
long currentTime = 0;
int counter = 0;

void setModel(Model*, float*);
void handleButtons(float *);

void setup() {
  lcd.begin();
  delay(500);

  radio.begin();

  controller.begin();

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
    lcd.setVolts(vmeter.getVoltage());
    telemetry.update();
    lcd.setRssi(telemetry.getRssi());
    lcd.setA1(telemetry.getA1());
    lcd.setA2(telemetry.getA2());
  }

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
}
