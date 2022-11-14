#include "display.h"

// #include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <esp_task_wdt.h>
#include <utils.h>

#include <queue>

#include "Retro8x16.c"

// VSPI pins on NodeMCU ESP32S
#define TFT_CLK 18
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 17

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

uint16_t screenBuffer[NES_SCREEN_WIDTH];
uint8_t *screenMemory[256 + 1];

// text position
uint8_t xPosOfText = 0;
uint8_t yPosOfText = 0;

// font
const char *displayFontSet = NULL;

QueueHandle_t vidQueue;

static void videoTask(void *arg);

void prepareVideoMemory();

void displayInit() {
  tft.begin();
  tft.setRotation(1);
  tft.setSPISpeed(80000000);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(100, TFT_HEIGHT / 2);
  tft.setTextSize(1);
  tft.printf("loading...");
  prepareVideoMemory();
  nescreen::setFont(Retro8x16);
  delay(200);
}

void initVideo() {
  //  disable Core 0 WDT
  TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
  esp_task_wdt_delete(idle_0);

  vidQueue = xQueueCreate(1, sizeof(uint8_t *));

  xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 0);
  debug("videoTask Pinned To Core 0...");
}

static void videoTask(void *arg) {
  while (1) {
    xQueueReceive(vidQueue, &screenMemory, portMAX_DELAY);
    nescreen::writeFrame(X_POS_OF_VIRTUAL_SCREEN, Y_POS_OF_VIRTUAL_SCREEN);
  }
}

void nescreen::update() { xQueueSend(vidQueue, &screenMemory, 0); }

void nescreen::drawPixel(uint8_t x, uint8_t y, uint8_t colorIndex) {
  if (y < NES_SCREEN_HEIGHT && x < NES_SCREEN_WIDTH) {
    screenMemory[y][x] = colorIndex;
  }
}

void nescreen::fillScreen(uint8_t colorIndex) {
  for (uint16_t y = 0; y < NES_SCREEN_HEIGHT; y++)
    for (uint16_t x = 0; x < NES_SCREEN_WIDTH; x++) {
      screenMemory[y][x] = colorIndex;
    }
}

void nescreen::writeFrame(const uint16_t x, const uint16_t y,
                          const uint16_t width, const uint16_t height) {
  uint8_t index;
  int16_t absYPos;

  for (uint16_t hLineIndex = 0; hLineIndex < height; hLineIndex++) {
    for (uint16_t X_ = 8; X_ < 248; X_++) {  // not 256 the 240
      index = (UNIVERSAL_BKG_COLOR & (screenMemory[hLineIndex])[X_]);
      screenBuffer[X_] = nes_16bit[index];
    }

    /// PAL optimalisation in this case:
    absYPos = y + hLineIndex;
    tft.drawRGBBitmap(x + 0, absYPos, screenBuffer, 48, 1);
    tft.drawRGBBitmap(x + 48, absYPos, screenBuffer + 48, 48, 1);
    tft.drawRGBBitmap(x + 96, absYPos, screenBuffer + 96, 48, 1);
    tft.drawRGBBitmap(x + 144, absYPos, screenBuffer + 144, 48, 1);
    tft.drawRGBBitmap(x + 192, absYPos, screenBuffer + 192, 48, 1);
  }
}

void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint8_t color) {
  for (uint16_t yPos = y; yPos < y + height; yPos++)
    for (uint16_t xPos = x; xPos < x + width; xPos++) {
      nescreen::drawPixel(xPos, yPos, color);
    }
}

void nescreen::drawLine(int16_t startX, int16_t startY, int16_t endX,
                        int16_t endY, uint8_t color) {
  int t, distance;
  int xerr = 0, yerr = 0, delta_x, delta_y;
  int incx, incy;
  // compute the distances in both directions
  delta_x = endX - startX;
  delta_y = endY - startY;
  // Compute the direction of the increment,
  //   an increment of 0 means either a horizontal or vertical
  //   line.
  if (delta_x > 0)
    incx = 1;
  else if (delta_x == 0)
    incx = 0;
  else
    incx = -1;

  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0;
  else
    incy = -1;

  // determine which distance is greater
  delta_x = abs(delta_x);
  delta_y = abs(delta_y);
  if (delta_x > delta_y)
    distance = delta_x;
  else
    distance = delta_y;

  // draw the line
  for (t = 0; t <= distance + 1; t++) {
    nescreen::drawPixel(startX, startY, color);

    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      startX += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      startY += incy;
    }
  }
}

void nescreen::drawHLine(int16_t startX, int16_t startY, int16_t endX,
                         uint8_t color) {
  for (uint16_t x = startX; x < endX; ++x) {
    nescreen::drawPixel(x, startY, color);
  }
}

void nescreen::drawVLine(int16_t startX, int16_t startY, int16_t endY,
                         uint8_t color) {
  for (uint16_t y = startY; y < endY; ++y) {
    nescreen::drawPixel(startX, y, color);
  }
}

void nescreen::drawRectangle(int16_t x, int16_t y, int16_t width,
                             int16_t height, uint8_t color) {
  nescreen::drawHLine(x, y, x + width, color);
  nescreen::drawVLine(x, y, y + height, color);
  nescreen::drawVLine(x + width, y, y + height, color);
  nescreen::drawHLine(x, y + height, x + width, color);
}

#define CHAR_SEGMENT_WITH 8
uint8_t nescreen::drawChar(uint16_t Main_x, uint16_t Main_y, char Main_char,
                           const char *font, uint8_t color, uint8_t bkgColor) {
  uint8_t charWidth = font[0];   // x char size
  uint8_t charHeight = font[1];  // y char size
  uint8_t charOffset = font[2];  // char start offset

  if (Main_char == '\n' || Main_char == '\r') {
    return charWidth;
  }

  uint8_t activeColor;
  uint8_t mask;
  uint8_t widthInSegments = charWidth / CHAR_SEGMENT_WITH;

  for (uint16_t yPos = 0; yPos < charHeight; yPos++)
    for (uint16_t xPos = 0; xPos < charWidth; xPos += CHAR_SEGMENT_WITH) {
      uint8_t charLine =
          font[(Main_char - charOffset) * (charHeight * widthInSegments) +
               (yPos)*widthInSegments + xPos / CHAR_SEGMENT_WITH + 4];

      // iterate from first (leading) to last bit
      // and draw pixel by pixel one segment of line
      mask = 1 << 7;  // 0b10000000
      for (uint8_t relX = xPos; relX < xPos + CHAR_SEGMENT_WITH; ++relX) {
        activeColor =
            (relX < charWidth) && (charLine & mask) != 0 ? color : bkgColor;
        nescreen::drawPixel(Main_x + relX, Main_y + yPos, activeColor);
        mask = mask >> 1;
      }
    }
  return charWidth;
}

uint8_t nescreen::drawString(uint8_t x, uint8_t y, const char *c, uint8_t color,
                             uint8_t bkgColor) {
  uint8_t width;
  uint8_t textWidth = 0;
  while (*c) {
    width = nescreen::drawChar(x, y, *c, displayFontSet, color, bkgColor);
    textWidth += (width);
    x += (width);
    c++;
  }
  setTextPosition(x, y);
  return textWidth;
}

uint8_t nescreen::drawString(const char *c, uint8_t color, uint8_t bkgColor) {
  return drawString(xPosOfText, yPosOfText, c, color, bkgColor);
}

void nescreen::drawText(const char *text, uint8_t color, uint8_t bkgColor) {
  if (text[strlen(text) - 1] == '\n') {
    nescreen::drawString(text, color, bkgColor);
    nescreen::setTextNewLine();
  } else {
    nescreen::drawString(text, color, bkgColor);
  }
}

void nescreen::setFont(const char *font) { displayFontSet = font; }

void nescreen::setTextPosition(uint8_t x, uint8_t y) {
  xPosOfText = x;
  yPosOfText = y;
}

void nescreen::setTextNewLine() {
  if (displayFontSet != NULL) {
    xPosOfText = 0;
    yPosOfText += (uint8_t)displayFontSet[1];
  }
}

// video memory allocation
void prepareVideoMemory() {
  for (uint8_t tmp = 0; tmp < 240; tmp++) {
    screenMemory[tmp] = (uint8_t *)malloc(256 + 1);
    memset(screenMemory[tmp], 0, 256);
  }
}