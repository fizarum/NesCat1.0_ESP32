#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#include <stdint.h>

#include <vector>

#include "../utils/utils.h"
#include "palette.h"
#include "rectangle.h"

class Sprite : public Rectangle {
 private:
  std::vector<ColorIndex> pixels;

 public:
  Sprite(uint8_t width, uint8_t height, uint8_t left = 0, uint8_t top = 0) {
    this->set(left, top, left + width - 1, top + height - 1);
  }
  ~Sprite() { pixels.clear(); }

  void setPixels(ColorIndex* pixels, uint16_t pixelCount) {
    this->pixels.clear();
    for (uint16_t index = 0; index < pixelCount; ++index) {
      this->pixels.push_back(pixels[index]);
    }
  }

  /**
   * @brief Get color index in sprite
   *
   * @param screenX - x coord on screen
   * @param screenY - y coord on screen
   * @param defaultValue - if color index cant be otained return this value
   * @return ColorIndex
   */
  ColorIndex getPixel(uint16_t screenX, uint16_t screenY,
                      ColorIndex defaultValue = COLOR_INDEX_UNDEF) {
    // translate from absolute to sprite relative coords
    uint8_t width = this->getWidth();
    uint8_t x = screenX - this->getLeft();
    uint8_t y = screenY - this->getTop();

    if (this->contains(screenX, screenY) == true) {
      uint16_t index = indexOf(x, y, width);
      return this->pixels[index];
    }
    return defaultValue;
  }
};

#endif  // GAME_SDK_SPRITE_H