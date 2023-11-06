#include "display.h"

#include <log.h>

#include "Monospaced16Bold.h"

// Stock font and GFXFF reference handle
#define GFXFF 1

#define MAIN_FONT &Monospaced_bold_16

TFT_eSPI tft = TFT_eSPI();

void DisplayDevice::onInit() {
  tft.init();
  // landscape
  tft.setRotation(1);
  tft.setCursor(100, 100, 4);
  tft.setTextColor(COLOR_WHITE);
  tft.fillScreen(COLOR_BLACK);
  tft.println("loading...");
}

void DisplayDevice::onUpdate() {
  // do nothing
}

void DisplayDevice::onEnabled(bool enabled) {
  // TODO: implement backlight control
}

void DisplayDevice::fillScreen(uint32_t color) {
  if (this->enabled() == false) return;
  tft.fillScreen(color);
}
void DisplayDevice::fillRectangle(int16_t x, int16_t y, int16_t width,
                                  int16_t height, uint32_t color) {
  if (this->enabled() == false) return;
  tft.fillRect(x, y, width, height, color);
}

void DisplayDevice::drawString(uint8_t x, uint8_t y, const char *c,
                               uint32_t color, uint8_t textPivot) {
  if (this->enabled() == false) return;

  tft.setFreeFont(MAIN_FONT);
  tft.setTextDatum(textPivot);
  tft.setTextColor(color);
  tft.drawString(c, x, y, GFXFF);
}

void DisplayDevice::setFontSize(uint8_t size) {
  if (this->enabled() == false) return;

  tft.setTextSize(size);
}
