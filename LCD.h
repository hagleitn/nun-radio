#ifndef LCD_h
#define LCD_h

#define OLED_RESET 4
#define LENGTH_NAME 7

#define SPACE 5
#define SEPARATOR 10

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "SPI.h"
#include "Battery.h"

class LCD {

 private:

  uint8_t volts;
  uint8_t rssi;
  uint8_t a1;
  uint8_t a2;
  char modelName[LENGTH_NAME+1] = {};
  int16_t *inputs;
  int16_t *channels;
  uint8_t numInputs;
  uint8_t numChannels;
  bool telemetryAvailable;
  Adafruit_SSD1306 display;

 private:

  void drawVolts(uint8_t v, uint8_t max, uint8_t min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b);
  void drawSignal(uint8_t v, uint8_t max, uint8_t min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b);
  void updateHeaders();
  void updateInputs();
  void updateChannels();

 public:

  LCD();

  inline void begin();
  inline void setTelemetryAvailable(bool available);
  inline void setVolts(uint8_t volts);
  inline void setRssi(uint8_t rssi);
  inline void setA1(uint8_t a1);
  inline void setA2(uint8_t a2);
  void setModelName(char const *name);
  void setInputs(int16_t *inputs, uint8_t n);
  void setChannels(int16_t *channels, uint8_t n);
#ifdef ENABLE_CALIBRATION
  void displayZeroCalibrationScreen();
  void displayMaxCalibrationScreen();
#endif
  void update();
};

inline LCD::LCD() : volts(0), rssi(0), a1(0), a2(0), numInputs(0), numChannels(0),
	   telemetryAvailable(false), display(OLED_RESET) {}

inline void LCD::begin() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
}

inline void LCD::setTelemetryAvailable(bool available) {
  this->telemetryAvailable = available;
}

inline void LCD::setVolts(uint8_t volts) {
  this->volts = volts;
}

inline void LCD::setRssi(uint8_t rssi) {
  this->rssi = rssi;
}

inline void LCD::setA1(uint8_t a1) {
  this->a1 = a1;
}

inline void LCD::setA2(uint8_t a2) {
  this->a2 = a2;
}

#endif
