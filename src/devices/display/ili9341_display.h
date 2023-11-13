#ifndef ILI9341_DISPLAY_H
#define ILI9341_DISPLAY_H

#include <display_device/display_device.h>

class ILI9341Display : public DisplayDevice {
 public:
  bool onInit();
  void onUpdate();
  void onEnabled(bool enabled);

  void fillScreen(uint16_t color);
  void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height,
                     uint16_t color);

  void drawString(uint8_t x, uint8_t y, const char* c,
                  uint16_t color = COLOR_WHITE, uint8_t textPivot = TL_DATUM);
  void setFontSize(uint8_t size);
};

#endif  // ILI9341_DISPLAY_H