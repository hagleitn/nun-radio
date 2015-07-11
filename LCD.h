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

#define MIN_CELL (3.5)
#define MAX_CELL (4.2)
#define CELL_COUNT (2)
#define LOAD_CORRECTION (0.2 * CELL_COUNT)
#define MIN_VOLTS (MIN_CELL * CELL_COUNT - LOAD_CORRECTION)
#define MAX_VOLTS (MAX_CELL * CELL_COUNT - LOAD_CORRECTION)

class LCD {

 private:

  float volts;
  uint8_t rssi;
  uint8_t a1;
  uint8_t a2;
  char modelName[LENGTH_NAME+1] = {};
  float *inputs;
  float *channels;
  uint8_t numInputs;
  uint8_t numChannels;
  Adafruit_SSD1306 display;

 private:

  void drawVolts(float v, float max, float min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b);
  void drawSignal(float v, float max, float min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b);
  void updateHeaders();
  void updateInputs();
  void updateChannels();

 public:

  LCD();

  inline void begin();
  inline void setVolts(float volts);
  inline void setRssi(uint8_t rssi);
  inline void setA1(uint8_t a1);
  inline void setA2(uint8_t a2);
  void setModelName(char const *name);
  void setInputs(float *inputs, uint8_t n);
  void setChannels(float *channels, uint8_t n);
  void update();
};

inline LCD::LCD() : volts(0), rssi(0), a1(0), a2(0), numInputs(0), numChannels(0), display(OLED_RESET) {}

inline void LCD::begin() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
}

inline void LCD::setVolts(float volts) {
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
