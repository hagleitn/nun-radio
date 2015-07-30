#include "LCD.h"

#include "Model.h"

#ifdef DEBUG
#define DEBUG_PRINT_NUMBERS 1
#endif

void LCD::drawVolts(uint8_t v, uint8_t max, uint8_t min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b) {
#ifdef DEBUG
  display.print(v);
  display.print("v ");
  display.print(min);
  display.print("v ");
  display.print(max);
  display.println("v");
#else
  if (v > max) v = max;
  if (v < min) v = min;
  display.fillRect(x,y,w,h,WHITE);
  display.fillRect(x+b, y+b, w-2*b, h-2*b, BLACK);
  display.fillRect(x+b+1, y+b+1, ((w-2*b-2) * (v-min)) / (max - min), h-2*b-2, WHITE);
#endif
  display.setCursor(x+w+5, y+2);
}

void LCD::drawSignal(uint8_t v, uint8_t max, uint8_t min, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t b) {
#ifdef DEBUG
  display.print(v);
  display.print(" ");
  display.print(min);
  display.print(" ");
  display.println(max);
#else
  if (v > max) v = max;
  if (v < min) v = min;

  uint8_t numBars = (uint8_t) ((v - min) * 6 / (max - min) + 0.5);
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
  drawVolts(this->volts, VOLTS_TO_BYTE(MAX_VOLTS_LIPO(2)),
            VOLTS_TO_BYTE(MIN_VOLTS_LIPO(2)), 0, 5, 18, 10, 1);

#ifndef DEBUG
  display.print(this->modelName);
#endif

  if (telemetryAvailable) {
    drawSignal(rssi, 100, 0, display.width() - 18, 5, 18, 10, 1);
    if (a1 != 0) {
      drawVolts(a1, VOLTS_TO_BYTE(MAX_VOLTS_NIMH(4)), VOLTS_TO_BYTE(MIN_VOLTS_NIMH(4)),
                display.width() - (18 + 5) * 2, 5, 18, 10, 1);
    }
    // currently not using a2. should make this configurable through model though
    // if (a2 != 0) {
    //   drawVolts(a2, VOLTS_TO_BYTE(MAX_VOLTS), VOLTS_TO_BYTE(MIN_VOLTS),
    //          display.width() - (18 + 5) * 3, 5, 18, 10, 1);
    // }
  } else {
    display.setCursor(display.width() - 55, 7);
    display.print("(no tele)");
  }
}

void LCD::updateInputs() {
#ifdef DEBUG_PRINT_NUMBERS
  display.println();
  for (uint8_t i = 0; i < numInputs; ++i) {
    display.print(inputs[i]);
    display.print(", ");
  }
#else
  uint8_t width = (display.width()/2 - (numInputs - 1) * SPACE - SEPARATOR) / numInputs;
  uint8_t height = display.height() - 24;

  for (uint8_t i = 0; i < numInputs; ++i) {
    uint8_t amp = ((inputs[i]+MAX_LEVEL) / 8) * height / ((MAX_LEVEL - MIN_LEVEL) / 8) + 1;
    display.fillRect(i*SPACE+i*width, display.height() - amp, width, amp, 1);
  }
#endif
}

void LCD::updateChannels() {
#ifdef DEBUG_PRINT_NUMBERS
  display.println();
  display.println();
  for (uint8_t i = 0; i < numChannels; ++i) {
    display.print(channels[i]);
    display.print(", ");
  }
#else
  uint8_t width = (display.width()/2 - (numChannels - 1) * SPACE - SEPARATOR) / numChannels;
  uint8_t height = display.height() - 24;
  uint8_t offset = display.width()/2 + SEPARATOR;

  for (uint8_t i = 0; i < numChannels; ++i) {
    uint8_t amp = ((channels[i]+MAX_LEVEL) / 8 ) * height / ((MAX_LEVEL - MIN_LEVEL) / 8) + 1;
    display.fillRect(offset + i*SPACE + i*width, display.height() - amp, width, amp, 1);
  }
#endif
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

void LCD::setInputs(int16_t *inputs, uint8_t n) {
  this->inputs = inputs;
  this->numInputs = n;
}

void LCD::setChannels(int16_t *channels, uint8_t n) {
  this->channels = channels;
  this->numChannels = n;
}

#ifdef ENABLE_CALIBRATION
void LCD::displayZeroCalibrationScreen(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);

  display.println("Calibration");
  display.println();
  display.println("Please center all axis and press C");
  display.display();
}

void LCD::displayMaxCalibrationScreen() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);

  display.println("Calibration");
  display.println();
  display.println("Please move all axis to max and min and press C");
  display.display();
}
#endif

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
