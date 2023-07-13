#ifndef DISPLAY_H
#define DISPLAY_H

#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240

#define H_CENTER DEFAULT_WIDTH / 2
#define V_CENTER DEFAULT_HEIGHT / 2

#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// Color definitions
#define COLOR_BLACK 0x0000        ///<   0,   0,   0
#define COLOR_NAVY 0x000F         ///<   0,   0, 123
#define COLOR_DARKGREEN 0x03E0    ///<   0, 125,   0
#define COLOR_DARKCYAN 0x03EF     ///<   0, 125, 123
#define COLOR_MAROON 0x7800       ///< 123,   0,   0
#define COLOR_PURPLE 0x780F       ///< 123,   0, 123
#define COLOR_OLIVE 0x7BE0        ///< 123, 125,   0
#define COLOR_LIGHTGREY 0xC618    ///< 198, 195, 198
#define COLOR_DARKGREY 0x7BEF     ///< 123, 125, 123
#define COLOR_BLUE 0x001F         ///<   0,   0, 255
#define COLOR_GREEN 0x07E0        ///<   0, 255,   0
#define COLOR_CYAN 0x07FF         ///<   0, 255, 255
#define COLOR_RED 0xF800          ///< 255,   0,   0
#define COLOR_MAGENTA 0xF81F      ///< 255,   0, 255
#define COLOR_YELLOW 0xFFE0       ///< 255, 255,   0
#define COLOR_WHITE 0xFFFF        ///< 255, 255, 255
#define COLOR_ORANGE 0xFD20       ///< 255, 165,   0
#define COLOR_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define COLOR_PINK 0xFC18         ///< 255, 130, 198
// custom colors
#define COLOR_VIOLET 0xEC1D     /// < 238,130,238
#define COLOR_DARK_GREY 0x632C  /// < 100,100,100

// screen buffer variables:
extern uint16_t screenBuffer[256];      // 512 bytes
extern uint8_t *screenMemory[256 + 1];  // 256*256 bytes + 256 offset

void displayInit();

void fillScreen(int color);
void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                   uint16_t color);

void drawString(uint8_t x, uint8_t y, const char *c,
                uint16_t color = COLOR_WHITE);
void setFontSize(uint8_t size);
#endif  // DISPLAY_H