#include <math.h>

#include <Wire.h>
#include "WiiChuck.h"
#include "Radio.h"
#include "Voltmeter.h"
#include "LCD.h"

WiiChuck chuck = WiiChuck();
Voltmeter vmeter = Voltmeter(A1, 100000.0, 30000.0);

// reta
float transform[2][4] = {{0,0.5,0,0.5},{0,0.5,0,-0.5}};
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

void setup() {

  Serial.begin(9600);
  Serial.println("I'm alive");

  radio = new Radio(1);

  chuck.begin();
  radio->setModel(&bee);

  //  chuck.calibrateJoy();
  chuck.update();

  lcd = new LCD();
  lcd->setModelName(bee.name);
  lcd->setRssi(0);
}

void normalize(float *x, int n) {
  for (int i = 0; i < n; ++i) {
    if (x[i] > 1) x[i] = 1;
    if (x[i] < -1) x[i] = -1;
  }
}

void loop() {
  delay(200);

  Serial.println("In loop");

  vmeter.update();
  lcd->setVolts(vmeter.getVoltage());

  chuck.update();

  /* Serial.print("Roll: "); */
  /* Serial.print(chuck.readRoll()); */

  /* Serial.print(", pitch:  "); */
  /* Serial.println(chuck.readPitch()); */

  /* Serial.print("z button: "); */
  /* Serial.print((int)chuck.zPressed()); */

  /* Serial.print(", c button: "); */
  /* Serial.println((int)chuck.cPressed()); */

  /* Serial.print("Joy (x): "); */
  /* Serial.print((int)chuck.readJoyX()); */

  /* Serial.print(", joy (y): "); */
  /* Serial.println((int)chuck.readJoyY()); */

  inputs[0] = chuck.readJoyX()/100.0;
  inputs[1] = chuck.readPitch()/90.0 - 1;
  inputs[2] = chuck.readJoyY()/100.0;
  inputs[3] = chuck.readRoll()/90.0;
  normalize(inputs, 4);

  for (int i = 0; i < 4; ++i) {
      Serial.print("Input: ");
      Serial.println(inputs[i]);
  }
  
  radio->send(inputs);
  float *channels = radio->getChannels();

  lcd->setInputs(inputs,4);
  lcd->setChannels(channels,2);

  lcd->update();
}
