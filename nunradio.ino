#include <math.h>

#include <Wire.h>
#include "Radio.h"
#include "Voltmeter.h"
#include "LCD.h"
#include "ModelRegistry.h"
#include "Telemetry.h"
#include "Controller.h"
#include "PermanentStore.h"

#ifdef ENABLE_ALARM
#include "Alarm.h"
#endif

Voltmeter vmeter(A1, 100000l, 30000l);
Telemetry telemetry;

#ifdef ENABLE_ALARM
Alarm alarm(13);
#endif

Controller controller;

#ifdef ENABLE_DUAL_RATES
Radio radio(5, 13);
#else
Radio radio(5);
#endif

LCD lcd;
ModelRegistry registry;

PermanentStore store;

unsigned long lastM = 0;
unsigned long currentTime = 0;
unsigned int counter = 0;

uint8_t volts[3] = {};
uint8_t signals[1] = {};

void setModel(Model*, int16_t*);
void handleButtons(int16_t *);

#define DELAY 10
#define ITERATIONS(ms) ((ms) / DELAY)

#define THRESHOLD (MAX_LEVEL * 7 / 10)

void setup() {

  // wait for things to stabilize
  delay(500);

#ifdef DEBUG
  Serial.begin(9600);
#endif

  lcd.begin();

  radio.begin();

  controller.begin();

  registry.begin();
  setModel(registry.current(), controller.getInputs());

  vmeter.begin();

  telemetry.begin();

#ifdef ENABLE_ALARM
  alarm.begin();
#endif
}

void loop() {
  delay(DELAY);
  currentTime = millis();

  if (counter % ITERATIONS(1000) == 0) {
    vmeter.update();
    volts[0] = vmeter.getVoltage();

    telemetry.update();

    volts[1] = telemetry.getA1();
    volts[2] = telemetry.getA2();
    signals[0] = telemetry.getRssi();

#ifdef DEBUG
    Serial.println("telemetry: ");
    Serial.println(telemetry.aquired());
    Serial.println(volts[0]);
    Serial.println(volts[1]);
    Serial.println(volts[2]);
    Serial.println(signals[0]);
#endif

    lcd.setTelemetryAvailable(telemetry.aquired());
    lcd.setVolts(volts[0]);
    lcd.setRssi(signals[0]);
    lcd.setA1(volts[1]);
    lcd.setA2(volts[2]);
  }

#ifdef ENABLE_ALARM
  if (counter % ITERATIONS(300) == 0) {
    alarm.update(currentTime, volts,
                 telemetry.aquired() ? 3 : 1, signals,
                 telemetry.aquired() ? 1 : 0);
  }
#endif


  controller.update(currentTime);

  handleButtons(controller.getInputs());

  radio.update(controller.getInputs());

  if (counter % ITERATIONS(200) == 0) {
    lcd.update();
  }

  ++counter;
}

void handleButtons(int16_t *inputs) {
  if (controller.bothPressed()) {
    if (currentTime - lastM > 500) {
      if (inputs[controller.getMode() ? 2 : 1] < -THRESHOLD) {
        lastM = currentTime;
        setModel(registry.next(), inputs);
      } else if (inputs[controller.getMode() ? 2 : 1] > THRESHOLD) {
        lastM = currentTime;
        setModel(registry.previous(), inputs);
      } else if (inputs[controller.getMode() ? 0 : 3] < -THRESHOLD) {
        lastM = currentTime;
        store.save(registry.current());
      } else if (inputs[controller.getMode() ? 0 : 3] > THRESHOLD) {
#ifdef ENABLE_CALIBRATION
	lcd.displayZeroCalibrationScreen();
	controller.calibrateZero();
	lcd.displayMaxCalibrationScreen();
	controller.calibrateMax();
#endif
      } else {
#ifdef ENABLE_DUAL_RATES
        radio.toggleRates();
#endif
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

void setModel(Model *m, int16_t *inputs) {
  store.load(m);

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
