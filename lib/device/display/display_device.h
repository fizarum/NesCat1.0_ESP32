#ifndef DISPLAY_H
#define DISPLAY_H

#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240

#define H_CENTER DEFAULT_WIDTH / 2
#define V_CENTER DEFAULT_HEIGHT / 2

#include <TFT_eSPI.h>

#include "../base_device.h"
#include "palette.h"

class DisplayDevice : public BaseDevice {
 public:
  bool onInit();
  void onUpdate();
  void onEnabled(bool enabled);

  void fillScreen(uint32_t color);
  void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                     uint32_t color);

  void drawString(uint8_t x, uint8_t y, const char *c,
                  uint32_t color = COLOR_WHITE, uint8_t textPivot = TL_DATUM);
  void setFontSize(uint8_t size);
};

#endif  // DISPLAY_H