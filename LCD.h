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

  void drawVolts(float v, float max, float min, int x, int y, int w, int h, int b) {
    float level = (v - min) / (max - min);
    if (level > 1) level = 1;
    if (level < 0) level = 0;
    display.fillRect(x,y,w,h,WHITE);
    display.fillRect(x+b, y+b, w-2*b, h-2*b, BLACK);
    display.fillRect(x+b+1, y+b+1, (w-2*b-2) * level, h-2*b-2, WHITE);
    display.setCursor(x+w+5, y+2);
  }

  void drawSignal(float v, float max, float min, int x, int y, int w, int h, int b) {
    float level = (v - min) / (max - min);
    if (level > 1) level = 1;
    if (level < 0) level = 0;
    int numBars = (int) (level * 6 + 0.5);
    int heightIncrement = h / 5;
    int barWidth = w/5 - b;

    int cur = x;

    for (int i = 0; i < numBars; ++i) {
      display.fillRect(cur, y+(4-i)*heightIncrement, barWidth, heightIncrement*(i+1),  WHITE);
      cur += barWidth + b;
    }
    display.setCursor(x+w+5, y+2);
  }

  void updateHeaders() {
    drawVolts(this->volts, MAX_VOLTS, MIN_VOLTS, 0, 5, 18, 10, 1);
    display.print(this->modelName);

    int i = 0;

    drawSignal(rssi, 100, 0, display.width() - 18, 5, 18, 10, 1);
    if (a1 != 0) {
      ++i;
      drawVolts(a2, MAX_VOLTS, MIN_VOLTS, display.width() - (18 + 5) * i, 5, 18, 10, 1);
    }
    if (a2 != 0) {
      ++i;
      drawVolts(a1, MAX_VOLTS, MIN_VOLTS, display.width() - (18 + 5) * i, 5, 18, 10, 1);
    }
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
