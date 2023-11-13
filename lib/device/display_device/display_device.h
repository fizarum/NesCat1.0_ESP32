#ifndef DISPLAY_DEVICE_H
#define DISPLAY_DEVICE_H

#include <base_device.h>
#include <stdint.h>

#include "datum.h"
#include "palette.h"

class DisplayDevice : public BaseDevice {
 public:
  const char* const getName() { return "display"; };
  virtual bool onInit() = 0;
  virtual void onUpdate() = 0;
  virtual void onEnabled(bool enabled){};

  virtual void fillScreen(uint16_t color) = 0;
  virtual void fillRectangle(int16_t x, int16_t y, int16_t width,
                             int16_t height, uint16_t color) = 0;

  virtual void drawString(uint8_t x, uint8_t y, const char* c,
                          uint16_t color = COLOR_WHITE,
                          uint8_t textPivot = TL_DATUM) = 0;

  virtual void setFontSize(uint8_t size) = 0;
};

#endif  // DISPLAY_DEVICE_H