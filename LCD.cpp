#include "LCD.h"

void LCD::drawVolts(uint8_t v, uint8_t max, uint8_t min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b) {
#ifdef DEBUG
  display.print(v);
#else
  float level = (v - min) / (max - min);
  if (level > 1) level = 1;
  if (level < 0) level = 0;
  display.fillRect(x,y,w,h,WHITE);
  display.fillRect(x+b, y+b, w-2*b, h-2*b, BLACK);
  display.fillRect(x+b+1, y+b+1, (w-2*b-2) * level, h-2*b-2, WHITE);
#endif
  display.setCursor(x+w+5, y+2);
}

void LCD::drawSignal(uint8_t v, uint8_t max, uint8_t min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b) {
#ifdef DEBUG
  display.print(v);
#else
  float level = (v - min) / (max - min);

  if (level > 1) level = 1;
  if (level < 0) level = 0;
  uint8_t numBars = (uint8_t) (level * 6 + 0.5);
  uint8_t heightIncrement = h / 5;
  uint8_t barWidth = w/5 - b;

  uint8_t cur = x;

  for (uint8_t i = 0; i < numBars; ++i) {
    display.fillRect(cur, y+(4-i)*heightIncrement, barWidth, heightIncrement*(i+1),  WHITE);
    cur += barWidth + b;
  }
#endif
  display.setCursor(x+w+5, y+2);
}

void LCD::updateHeaders() {
  drawVolts(this->volts, VOLTS_TO_BYTE(MAX_VOLTS), VOLTS_TO_BYTE(MIN_VOLTS), 0, 5, 18, 10, 1);
  display.print(this->modelName);

  uint8_t i = 0;

  if (telemetryAvailable) {
    drawSignal(rssi, 100, 0, display.width() - 18, 5, 18, 10, 1);
    if (a1 != 0) {
      ++i;
      drawVolts(a2, VOLTS_TO_BYTE(MAX_VOLTS), VOLTS_TO_BYTE(MIN_VOLTS),
		display.width() - (18 + 5) * i, 5, 18, 10, 1);
    }
    if (a2 != 0) {
      ++i;
      drawVolts(a1, VOLTS_TO_BYTE(MAX_VOLTS), VOLTS_TO_BYTE(MIN_VOLTS),
		display.width() - (18 + 5) * i, 5, 18, 10, 1);
    }
  } else {
    display.setCursor(display.width() - 55, 7);
    display.print("(no tele)");
  }
}

void LCD::updateInputs() {
  uint8_t width = (display.width()/2 - (numInputs - 1) * SPACE - SEPARATOR) / numInputs;
  uint8_t height = (display.height() - 24)/2;

  for (uint8_t i = 0; i < numInputs; ++i) {
    uint8_t amp = (inputs[i]+1)*height +1;
    display.fillRect(i*SPACE+i*width, display.height() - amp, width, amp, 1);
  }
}

void LCD::updateChannels() {
  uint8_t width = (display.width()/2 - (numChannels - 1) * SPACE - SEPARATOR) / numChannels;
  uint8_t height = (display.height() - 24)/2;
  uint8_t offset = display.width()/2 + SEPARATOR;

  for (uint8_t i = 0; i < numChannels; ++i) {
    uint8_t amp = (channels[i]+1)*height +1;
    display.fillRect(offset + i*SPACE + i*width, display.height() - amp, width, amp, 1);
  }
}

void LCD::setModelName(char const *name) {
  uint8_t i = 0;
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

void LCD::setInputs(float *inputs, uint8_t n) {
  this->inputs = inputs;
  this->numInputs = n;
}

void LCD::setChannels(float *channels, uint8_t n) {
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
