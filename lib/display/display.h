#ifndef DISPLAY_H
#define DISPLAY_H

#include "nes_palettes.h"

// VSPI pins on NodeMCU ESP32S
#define TFT_CLK 18
#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_CS 5
#define TFT_DC 16
#define TFT_RST 17

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

#define NES_SCREEN_WIDTH 256
#define NES_SCREEN_HEIGHT 240

// nes universal background color, more:
// https://emudev.de/nes-emulator/palettes-attribute-tables-and-sprites/ or
// https://helloacm.com/tutorial-7-c-programming-in-6502-colour-setting-for-nes/
#define UNIVERSAL_BKG_COLOR 0x3f

#include <SPI.h>

void displayInit();

void displayDrawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[],
                          int16_t w, int16_t h);

// Allocated MEMORY variables:
extern uint16_t screenBuffer[256];      // 512 bytes
extern uint8_t *screenMemory[256 + 1];  // 256*256 bytes + 256 offset

#endif  // DISPLAY_H