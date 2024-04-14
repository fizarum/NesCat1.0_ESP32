#ifndef DISPLAY_DEVICE_H
#define DISPLAY_DEVICE_H

#include <stdint.h>

#include "../base_device.h"
#include "datum.h"
#include "palette.h"

class DisplayDevice : public BaseDevice {
 private:
  uint8_t backlightValue;

 public:
  const char* const getName() { return "display"; };
  virtual bool onInit() = 0;
  virtual void onUpdate() = 0;
  virtual void onEnabled(bool enabled) {};
  virtual void onBacklightChanged(uint8_t value) = 0;

  /**
   * @brief Set the Back Light value
   *
   * @param newValue (BL_LEVEL_MIN..BL_LEVEL_MAX)
   * BL_LEVEL_MAX - full light
   * BL_LEVEL_MIN - turn off backlight
   */
  virtual void setBackLightValue(uint8_t newValue) final {
    if (newValue >= BL_LEVEL_MIN && newValue <= BL_LEVEL_MAX) {
      this->backlightValue = newValue;
      onBacklightChanged(newValue);
    }
  }
  uint8_t getBacklightValue() { return backlightValue; }

  virtual void fillScreen(uint16_t color) = 0;
  virtual void fillRectangle(int16_t x, int16_t y, int16_t width,
                             int16_t height, uint16_t color) = 0;

  virtual void drawString(uint8_t x, uint8_t y, const char* c,
                          uint16_t color = COLOR_WHITE,
                          uint8_t textPivot = TL_DATUM) = 0;

  virtual void setFontSize(uint8_t size) = 0;
};

#endif  // DISPLAY_DEVICE_H