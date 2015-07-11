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
  int rssi;
  int a1;
  int a2;
  char modelName[LENGTH_NAME+1] = {};
  float *inputs;
  float *channels;
  int numInputs;
  int numChannels;
  Adafruit_SSD1306 display;

 private:

  void drawVolts(float v, float max, float min, int x, int y, int w, int h, int b);
  void drawSignal(float v, float max, float min, int x, int y, int w, int h, int b);
  void updateHeaders();
  void updateInputs();
  void updateChannels();

 public:

  LCD();

  inline void begin();
  inline void setVolts(float volts);
  inline void setRssi(int rssi);
  inline void setA1(int a1);
  inline void setA2(int a2);
  void setModelName(char *name);
  void setInputs(float *inputs, int n);
  void setChannels(float *channels, int n);
  void update();
};

inline LCD::LCD() : display(OLED_RESET) {}

inline void LCD::begin() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
}

inline void LCD::setVolts(float volts) {
  this->volts = volts;
}

inline void LCD::setRssi(int rssi) {
  this->rssi = rssi;
}

inline void LCD::setA1(int a1) {
  this->a1 = a1;
}

inline void LCD::setA2(int a2) {
  this->a2 = a2;
}

#endif
