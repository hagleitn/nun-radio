#include "LCD.h"

void LCD::drawVolts(float v, float max, float min, int x, int y, int w, int h, int b) {
  float level = (v - min) / (max - min);
  if (level > 1) level = 1;
  if (level < 0) level = 0;
  display.fillRect(x,y,w,h,WHITE);
  display.fillRect(x+b, y+b, w-2*b, h-2*b, BLACK);
  display.fillRect(x+b+1, y+b+1, (w-2*b-2) * level, h-2*b-2, WHITE);
  display.setCursor(x+w+5, y+2);
}

void LCD::drawSignal(float v, float max, float min, int x, int y, int w, int h, int b) {
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

void LCD::updateHeaders() {
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

void LCD::updateInputs() {
  int width = (display.width()/2 - (numInputs - 1) * SPACE - SEPARATOR) / numInputs;
  int height = (display.height() - 24)/2;

  for (int i = 0; i < numInputs; ++i) {
    int amp = (inputs[i]+1)*height +1;
    display.fillRect(i*SPACE+i*width, display.height() - amp, width, amp, 1);
  }
}

void LCD::updateChannels() {
  int width = (display.width()/2 - (numChannels - 1) * SPACE - SEPARATOR) / numChannels;
  int height = (display.height() - 24)/2;
  int offset = display.width()/2 + SEPARATOR;

  for (int i = 0; i < numChannels; ++i) {
    int amp = (channels[i]+1)*height +1;
    display.fillRect(offset + i*SPACE + i*width, display.height() - amp, width, amp, 1);
  }
}

void LCD::setModelName(char *name) {
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

void LCD::setInputs(float *inputs, int n) {
  this->inputs = inputs;
  this->numInputs = n;
}

void LCD::setChannels(float *channels, int n) {
  this->channels = channels;
  this->numChannels = n;
}

void LCD::update() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  updateHeaders();
  updateInputs();
  updateChannels();
  display.display();
}
