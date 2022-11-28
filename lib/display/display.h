#ifndef DISPLAY_H
#define DISPLAY_H

#include "nes_palettes.h"

#define NES_SCREEN_WIDTH 256
#define NES_SCREEN_HEIGHT 240

#define DEFAULT_WIDTH NES_SCREEN_WIDTH
#define DEFAULT_HEIGHT NES_SCREEN_HEIGHT

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

//********************************************************************************
/**
 * @brief position of virtual screen (nes) on real screen size
 * x & y should be 0 in ideal case, but aspect ratio of screen is bigger (wider)
 * than emulator one, then: x > 0
 */
#define X_POS_OF_VIRTUAL_SCREEN (TFT_WIDTH - DEFAULT_WIDTH) / 2
#define Y_POS_OF_VIRTUAL_SCREEN (TFT_HEIGHT - DEFAULT_HEIGHT) / 2

#define H_CENTER NES_SCREEN_WIDTH / 2
#define V_CENTER NES_SCREEN_HEIGHT / 2

// nes universal background color, more:
// https://emudev.de/nes-emulator/palettes-attribute-tables-and-sprites/ or
// https://helloacm.com/tutorial-7-c-programming-in-6502-colour-setting-for-nes/
#define UNIVERSAL_BKG_COLOR 0x3f
// menu part
#define MENU_TEXT_COLOR 0x30
#define MENU_BACKGROUND_COLOR 0x0c
#define ACCENT_COLOR 0x16

#include <SPI.h>

// screen buffer variables:
extern uint16_t screenBuffer[256];      // 512 bytes
extern uint8_t *screenMemory[256 + 1];  // 256*256 bytes + 256 offset

extern QueueHandle_t vidQueue;

void displayInit();

void initVideo();

// video memory namespace
namespace nescreen {
void update();

void drawPixel(uint8_t X, uint8_t Y, uint8_t colorIndex);

void fillScreen(uint8_t colorIndex = UNIVERSAL_BKG_COLOR);

void writeFrame(const uint16_t x, const uint16_t y,
                const uint16_t width = DEFAULT_WIDTH,
                const uint16_t height = DEFAULT_HEIGHT);

void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint8_t color);

void drawLine(int16_t startX, int16_t startY, int16_t endX, int16_t endY,
              uint8_t color);

void drawHLine(int16_t startX, int16_t startY, int16_t endX, uint8_t color);

void drawVLine(int16_t startX, int16_t startY, int16_t endY, uint8_t color);

void drawRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint8_t color);

uint8_t drawChar(uint16_t Main_x, uint16_t Main_y, char Main_char,
                 const char *font, uint8_t color, uint8_t bkgColor);

uint8_t drawString(uint8_t x, uint8_t y, const char *c, uint8_t color,
                   uint8_t bkgColor);

uint8_t drawString(const char *c, uint8_t color, uint8_t bkgColor);

void drawText(const char *text, uint8_t color = MENU_TEXT_COLOR,
              uint8_t bkgColor = MENU_BACKGROUND_COLOR);

void setFont(const char *font);

void setTextPosition(uint8_t x, uint8_t y);
void setTextNewLine();

};  // namespace nescreen

#endif  // DISPLAY_H