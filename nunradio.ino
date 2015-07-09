#include <math.h>

#include <Wire.h>
#include "WiiChuck.h"
#include "Radio.h"
#include "Voltmeter.h"
#include "LCD.h"
#include "ModelRegistry.h"

WiiChuck chuck;
Voltmeter vmeter(A1, 100000.0, 30000.0);

Radio radio(5);
float inputs[4];

LCD lcd;
ModelRegistry registry;

bool zPressed, cPressed;

int counter = 0;
long lastM = -1000;
long lastZ = -1000;
long lastC = -1000;
long currentTime = 0;

bool mode2 = true;

void setModel(Model *m) {
  radio.setModel(m);
  lcd.setChannels(radio.getChannels(), m->numChannels);
  lcd.setModelName(m->name);
  lcd.setRssi(0);
  lcd.setInputs(inputs, m->numInputs);
}

void setup() {
  lcd.begin();
  delay(500);

  radio.begin();

  registry.begin();
  setModel(registry.current());

  chuck.begin();
  chuck.update();

  vmeter.begin();
}

void normalize(float *x, int n) {
  for (int i = 0; i < n; ++i) {
    if (x[i] > 1) x[i] = 1;
    if (x[i] < -1) x[i] = -1;
  }
}

void setInputs() {
  chuck.update();
  if (mode2) {
    inputs[0] = chuck.readJoyX()/100.0;
    inputs[1] = chuck.readPitch()/90.0 - 1;
    inputs[2] = chuck.readJoyY()/100.0;
    inputs[3] = chuck.readRoll()/90.0;
  } else {
    inputs[3] = chuck.readJoyX()/100.0;
    inputs[2] = chuck.readPitch()/90.0 - 1;
    inputs[1] = chuck.readJoyY()/100.0;
    inputs[0] = chuck.readRoll()/90.0;
  }
  normalize(inputs, 4);
}

void handleButtons() {
  zPressed = chuck.zPressed();
  cPressed = chuck.cPressed();

  if (zPressed) {
    lastZ = currentTime;
  }

  if (cPressed) {
    lastC = currentTime;
  }

  if (zPressed && cPressed) {
    if (currentTime - lastM > 500) {
      if (inputs[mode2 ? 2 : 1] < -0.7) {
        lastM = currentTime;
        setModel(registry.next());
      } else if (inputs[mode2 ? 2 : 1] > 0.7) {
        lastM = currentTime;
        setModel(registry.previous());
      }
    }
  }

  if (lastZ - currentTime > 100
      && lastZ - currentTime < 200
      && lastC - currentTime > 200) {
    lastZ = -1000;
    mode2 = !mode2;
  }

  if (lastC - currentTime > 100
      && lastC - currentTime > 100
      && lastZ - currentTime > 200) {
    lastC = -1000;
    radio.setTrim(inputs, registry.current()->numInputs);
  }
}

void loop() {
  delay(50);
  currentTime = millis();
  ++counter;

  if (counter % 20 == 0) {
    vmeter.update();
    lcd.setVolts(vmeter.getVoltage());
  }

  setInputs();

  handleButtons();

  radio.send(inputs);

  if (counter % 10 == 0) {
    lcd.update();
  }
}
