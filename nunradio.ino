#include <math.h>

#include <Wire.h>
#include "WiiChuck.h"
#include "Radio.h"
#include "Voltmeter.h"
#include "LCD.h"

WiiChuck chuck = WiiChuck();
Voltmeter vmeter = Voltmeter(A1, 100000.0, 30000.0);

// reta
float transform[2][4] = {{0,0.5,0,0.5},{0,-0.5,0,0.5}};
Model bee = {
  "bee",
  (float[]){0,0.3,0,0.3},
  (float[]){0.8,0.8},
  (float[]){1,1},
  (float*)transform,
  2,
  4
};

Radio *radio;
float inputs[4];

LCD *lcd;

int counter = 0;

void setup() {

  //  Serial.begin(9600);

  lcd = new LCD();
  delay(500);

  lcd->setModelName(bee.name);
  lcd->setRssi(0);
  lcd->setInputs(inputs, bee.numInputs);

  radio = new Radio(5);
  radio->setModel(&bee);
  lcd->setChannels(radio->getChannels(), bee.numChannels);

  //  chuck.calibrateJoy();
  chuck.begin();
  chuck.update();
}

void normalize(float *x, int n) {
  for (int i = 0; i < n; ++i) {
    if (x[i] > 1) x[i] = 1;
    if (x[i] < -1) x[i] = -1;
  }
}

void loop() {
  delay(50);
  ++counter;

  if (counter % 20 == 0) {
    vmeter.update();
    lcd->setVolts(vmeter.getVoltage());
  }

  chuck.update();

  inputs[0] = chuck.readJoyX()/100.0;
  inputs[1] = chuck.readPitch()/90.0 - 1;
  inputs[2] = chuck.readJoyY()/100.0;
  inputs[3] = chuck.readRoll()/90.0;
  normalize(inputs, 4);

  radio->send(inputs);
  float *channels = radio->getChannels();

  if (counter % 10 == 0) {
    lcd->update();
  }
}
