#include "ili9341_display.h"

#include <TFT_eSPI.h>
#include <log.h>

#include "Monospaced16Bold.h"

// Stock font and GFXFF reference handle
#define GFXFF 1

#define MAIN_FONT &Monospaced_bold_16

TFT_eSPI tft = TFT_eSPI();

bool ILI9341Display::onInit() {
  tft.init();
  // landscape
  tft.setRotation(DISPLAY_ORIENTATION);
  tft.setCursor(100, 100, 4);
  tft.setTextColor(COLOR_WHITE);
  tft.fillScreen(COLOR_BLACK);
  tft.println("loading...");

  return true;
}

void ILI9341Display::onUpdate() {
  // do nothing
}

void ILI9341Display::onEnabled(bool enabled) {
  // TODO: implement backlight control
}

void ILI9341Display::fillScreen(uint16_t color) {
  if (this->enabled() == false) return;
  tft.fillScreen(color);
}
void ILI9341Display::fillRectangle(int16_t x, int16_t y, int16_t width,
                                   int16_t height, uint16_t color) {
  if (this->enabled() == false) return;
  tft.fillRect(x, y, width, height, color);
}

void ILI9341Display::drawString(uint8_t x, uint8_t y, const char* c,
                                uint16_t color, uint8_t textPivot) {
  if (this->enabled() == false) return;

  tft.setFreeFont(MAIN_FONT);
  tft.setTextDatum(textPivot);
  tft.setTextColor(color);
  tft.drawString(c, x, y, GFXFF);
}

void ILI9341Display::setFontSize(uint8_t size) {
  if (this->enabled() == false) return;

  tft.setTextSize(size);
}
