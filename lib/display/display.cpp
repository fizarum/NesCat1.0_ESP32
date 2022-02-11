#include "display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

uint16_t screenBuffer[256];
uint8_t *screenMemory[256 + 1];

void displayInit() {
  tft.begin();
  tft.setRotation(1);
  tft.setSPISpeed(80000000);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(100, TFT_HEIGHT / 2);
  tft.setTextSize(1);
  tft.printf("loading...");
}

void displayDrawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[],
                          int16_t w, int16_t h) {
  tft.drawRGBBitmap(x, y, bitmap, w, h);
}