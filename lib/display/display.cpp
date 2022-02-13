#include "display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

uint16_t screenBuffer[256];
uint8_t *screenMemory[256 + 1];

// text position
uint8_t xPosOfText = 0;
uint8_t yPosOfText = 0;

// font
const char *displayFontSet = NULL;

void displayInit() {
  tft.begin();
  tft.setRotation(1);
  tft.setSPISpeed(80000000);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(100, TFT_HEIGHT / 2);
  tft.setTextSize(1);
  tft.printf("loading...");
}

void nescreen::drawPixel(uint8_t X, uint8_t Y, uint8_t colorIndex) {
  if (Y < NES_SCREEN_HEIGHT && X < NES_SCREEN_WIDTH) {
    screenMemory[Y][X] = colorIndex;
  }
}

void nescreen::fillscreen(uint8_t colorIndex) {
  for (uint16_t y = 0; y < NES_SCREEN_HEIGHT; y++)
    for (uint16_t x = 0; x < NES_SCREEN_WIDTH; x++) {
      screenMemory[y][x] = colorIndex;
    }
}

void nescreen::writeFrame(const uint16_t x, const uint16_t y,
                          const uint16_t width, const uint16_t height) {
  uint8_t index;
  int16_t absYPos;

  for (uint16_t i = 0; i < height; i++) {
    for (uint16_t X_ = 8; X_ < 248; X_++) {  // not 256 the 240
      index = (UNIVERSAL_BKG_COLOR & (screenMemory[i])[X_]);
      screenBuffer[X_] = nes_16bit[index];
    }

    /// PAL optimalisation in this case:
    absYPos = y + i;
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

uint8_t nescreen::drawChar(uint16_t Main_x, uint16_t Main_y, char Main_char,
                           const char *font, uint8_t color) {
  uint8_t charWidth = font[0];   // x char size
  uint8_t charHeight = font[1];  // y char size
  uint8_t charOffset = font[2];  // char start offset

  if (Main_char != '\n' && Main_char != '\r')
    for (uint16_t Ypos = 0; Ypos < charHeight; Ypos++)
      for (uint16_t Xpos = 0; Xpos < charWidth; Xpos += 8) {
        uint8_t CHARLINE =
            font[(Main_char - charOffset) * (charHeight * (charWidth / 8)) +
                 (Ypos) * (charWidth / 8) + Xpos / 8 + 4];

        if ((Xpos + 0 < charWidth) && (CHARLINE & 0b10000000) != 0)
          nescreen::drawPixel(Main_x + Xpos + 0, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 0, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 1 < charWidth) && (CHARLINE & 0b01000000) != 0)
          nescreen::drawPixel(Main_x + Xpos + 1, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 1, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 2 < charWidth) && (CHARLINE & 0b00100000) != 0)
          nescreen::drawPixel(Main_x + Xpos + 2, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 2, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 3 < charWidth) && (CHARLINE & 0b00010000) != 0)
          nescreen::drawPixel(Main_x + Xpos + 3, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 3, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 4 < charWidth) && (CHARLINE & 0b00001000) != 0)
          nescreen::drawPixel(Main_x + Xpos + 4, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 4, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 5 < charWidth) && (CHARLINE & 0b00000100) != 0)
          nescreen::drawPixel(Main_x + Xpos + 5, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 5, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 6 < charWidth) && (CHARLINE & 0b00000010) != 0)
          nescreen::drawPixel(Main_x + Xpos + 6, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 6, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
        if ((Xpos + 7 < charWidth) && (CHARLINE & 0b00000001) != 0)
          nescreen::drawPixel(Main_x + Xpos + 7, Main_y + Ypos, color);
        else
          nescreen::drawPixel(Main_x + Xpos + 7, Main_y + Ypos,
                              UNIVERSAL_BKG_COLOR);
      }
  return charWidth;
}

uint8_t nescreen::drawString(uint8_t x, uint8_t y, const char *c,
                             uint8_t color) {
  uint8_t width;
  uint8_t textwidth = 0;
  while (*c) {
    width = nescreen::drawChar(x, y, *c, displayFontSet, color);
    textwidth += (width);
    x += (width);
    c++;
  }
  setTextPosition(x, y);
  return textwidth;
}

uint8_t nescreen::drawString(const char *c, uint8_t color) {
  return drawString(xPosOfText, yPosOfText, c, color);
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