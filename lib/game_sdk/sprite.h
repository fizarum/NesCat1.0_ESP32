#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#include <stdint.h>

#include <vector>

#include "../utils/utils.h"
#include "palette.h"
#include "rectangle.h"

// TODO: reuse rectangle class
class Sprite {
 private:
  Rectangle bounds;
  std::vector<ColorIndex> pixels;

 public:
  Sprite(uint8_t width, uint8_t height, uint8_t left = 0, uint8_t top = 0) {
    this->bounds.set(left, top, left + width - 1, top + height - 1);
  }
  ~Sprite() { pixels.clear(); }

  uint8_t speed;
  bool positionChanged;

  uint8_t getWidth() { return bounds.getWidth(); }
  uint8_t getHeight() { return bounds.getHeight(); }
  int16_t getLeft() { return bounds.getLeft(); }
  int16_t getTop() { return bounds.getTop(); }
  int16_t getRight() { return bounds.getRight(); }
  int16_t getBottom() { return bounds.getBottom(); }

  void setPixels(ColorIndex* pixels, uint16_t pixelCount) {
    this->pixels.clear();
    for (uint16_t index = 0; index < pixelCount; ++index) {
      this->pixels.push_back(pixels[index]);
    }
  }

  ColorIndex getPixel(uint16_t screenX, uint16_t screenY) {
    // translate from absolute to sprite relative coords
    uint8_t width = bounds.getWidth();
    uint8_t x = screenX - bounds.getLeft();
    uint8_t y = screenY - bounds.getTop();

    uint16_t index = indexOf(x, y, width);
    return pixels[index];
  }

  // checks if by [x,y] sprite has such pixel and return it color index
  ColorIndex getPixelOrDefault(uint8_t screenX, uint8_t screenY,
                               ColorIndex defaultValue) {
    if (bounds.contains(screenX, screenY) == true) {
      return getPixel(screenX, screenY);
    }
    return defaultValue;
  }

  Rectangle* getBounds() { return &bounds; }

  void moveBy(int8_t x, int8_t y) {
    // TODO: add bounds checking
    // TODO: use speed
    bounds.moveBy(x, y);
    positionChanged = true;
  }

  void moveTo(uint8_t x, uint8_t y) {
    bounds.moveTo(x, y);
    positionChanged = true;
  }

  /**
   * @brief checks if x & y are inside of sprite
   *
   * @param x coord on screen
   * @param y coord on screen
   * @return true if pixel is inside of sprite
   * @return false otherwise
   */
  bool contains(uint8_t x, uint8_t y) { return bounds.contains(x, y); }
};

#endif  // GAME_SDK_SPRITE_H