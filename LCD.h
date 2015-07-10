#ifndef LCD_h
#define LCD_h

#define OLED_RESET 4
#define LENGTH_NAME 3

#define SPACE 5
#define SEPARATOR 10

#include "Arduino.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "SPI.h"

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

 public:

  LCD() : display(OLED_RESET) {}

  void begin() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
  }

  void setVolts(float volts) {
    this->volts = volts;
  }

  void setRssi(int rssi) {
    this->rssi = rssi;
  }

  void setA1(int a1) {
    this->a1 = a1;
  }

  void setA2(int a2) {
    this->a2 = a2;
  }

  void setModelName(char *name) {
    int i = 0;
    for (; i < LENGTH_NAME; ++i) {
      if (name[i] == '\0') {
        break;
      }
      this->modelName[i] = name[i];
    }
    for (; i < LENGTH_NAME; ++i) {
      this->modelName[i] = ' ';
    }
  }

  void setInputs(float *inputs, int n) {
    this->inputs = inputs;
    this->numInputs = n;
  }

  void setChannels(float *channels, int n) {
    this->channels = channels;
    this->numChannels = n;
  }

  void updateHeaders() {
    display.print(this->volts);
    display.print("V ");
    display.print(this->modelName);
    display.print(" ");
    if (a1 != 0) {
      display.print(a1);
      display.print(" ");
    }
    if (a2 != 0) {
      display.print(a2);
      display.print(" ");
    }
    display.println(rssi);
  }

  void updateInputs() {
    int width = (display.width()/2 - (numInputs - 1) * SPACE - SEPARATOR) / numInputs;
    int height = (display.height() - 24)/2;

    for (int i = 0; i < numInputs; ++i) {
      int amp = (inputs[i]+1)*height +1;
      display.fillRect(i*SPACE+i*width, display.height() - amp, width, amp, 1);
    }
  }

  void updateChannels() {
    int width = (display.width()/2 - (numChannels - 1) * SPACE - SEPARATOR) / numChannels;
    int height = (display.height() - 24)/2;
    int offset = display.width()/2 + SEPARATOR;

    for (int i = 0; i < numChannels; ++i) {
      int amp = (channels[i]+1)*height +1;
      display.fillRect(offset + i*SPACE + i*width, display.height() - amp, width, amp, 1);
    }
  }

  void update() {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    updateHeaders();
    updateInputs();
    updateChannels();
    display.display();
  }
};

#endif
