#ifndef NES_DISPLAY_H
#define NES_DISPLAY_H

#include <configurator.h>
#include <display_device/display_device.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "nes_palettes.h"

#define NES_SCREEN_WIDTH 256
#define NES_SCREEN_HEIGHT 240

//********************************************************************************
/**
 * @brief position of virtual screen (nes) on real screen size
 * x & y should be 0 in ideal case, but aspect ratio of screen is bigger (wider)
 * than emulator one, then: x > 0
 */
#define X_POS_OF_VIRTUAL_SCREEN (DISPLAY_WIDTH - NES_SCREEN_WIDTH) / 2
#define Y_POS_OF_VIRTUAL_SCREEN (DISPLAY_HEIGHT - NES_SCREEN_HEIGHT) / 2

extern QueueHandle_t vidQueue;

namespace nescreen {

void initVideo(/*Adafruit_SPITFT *tft*/);
void update();

void drawPixel(uint8_t X, uint8_t Y, uint8_t colorIndex);

void fillScreen(uint8_t colorIndex = UNIVERSAL_BKG_COLOR);

void writeFrame(uint16_t x, uint16_t y, uint16_t width = DISPLAY_WIDTH,
                uint16_t height = DISPLAY_HEIGHT);

void drawLine(int16_t startX, int16_t startY, int16_t endX, int16_t endY,
              uint8_t color);

void drawHLine(int16_t startX, int16_t startY, int16_t endX, uint8_t color);

void drawVLine(int16_t startX, int16_t startY, int16_t endY, uint8_t color);

void drawRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint8_t color);

void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint8_t color);

uint8_t drawChar(uint16_t Main_x, uint16_t Main_y, char Main_char,
                 const char *font, uint8_t color, uint8_t bkgColor);

uint8_t drawString(uint8_t x, uint8_t y, const char *c, uint8_t color,
                   uint8_t bkgColor);

uint8_t drawString(const char *c, uint8_t color, uint8_t bkgColor);

void drawText(const char *text, uint8_t color = WHITE_COLOR,
              uint8_t bkgColor = BLUE_COLOR);

void setFont(const char *font);

void setTextPosition(uint8_t x, uint8_t y);
void setTextNewLine();
}  // namespace nescreen

#endif  // NES_DISPLAY_H