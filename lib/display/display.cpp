#include "display.h"

#include <Adafruit_ILI9341.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <esp_task_wdt.h>
#include <utils.h>

#include <queue>

// VSPI pins on NodeMCU ESP32S
#define TFT_CLK 18
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 17

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// text position
uint8_t xPosOfText = 0;
uint8_t yPosOfText = 0;

void displayInit() {
  tft.begin();
  tft.setRotation(1);
  tft.setSPISpeed(80000000);
  tft.fillScreen(COLOR_BLACK);
  tft.setCursor(100, V_CENTER);
  tft.setTextSize(1);
  tft.setFont(&FreeMonoBold9pt7b);
  tft.printf("loading...");
  delay(200);
}

void update() {}

void fillScreen(int color) { tft.fillScreen(color); }
void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint16_t color) {
  tft.fillRect(x, y, width, height, color);
}

void drawString(uint8_t x, uint8_t y, const char *c, uint16_t color) {
  tft.setCursor(x, y);
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.printf(c);
}

void drawString(const char *c, uint16_t color) {
  return drawString(xPosOfText, yPosOfText, c, color);
}
