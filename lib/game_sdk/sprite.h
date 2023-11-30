#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#include <palette.h>
#include <stdint.h>

#include <vector>

class Sprite {
 private:
  int16_t left;
  int16_t top;
  int16_t right;
  int16_t bottom;
  uint8_t width;
  uint8_t height;

  std::vector<ColorIndex> pixels;

 public:
  Sprite(uint8_t width, uint8_t height, uint8_t left = 0, uint8_t top = 0) {
    this->left = left;
    this->top = top;
    this->right = left + width - 1;
    this->bottom = top + height - 1;
    this->width = width;
    this->height = height;
  }

  uint8_t getWidth() { return width; }
  uint8_t getHeight() { return height; }
  int16_t getLeft() { return left; }
  int16_t getTop() { return top; }
  int16_t getRight() { return right; }
  int16_t getBottom() { return bottom; }

  void setPixels(ColorIndex* pixels, uint16_t pixelCount) {
    this->pixels.clear();
    for (uint16_t index = 0; index < pixelCount; ++index) {
      this->pixels.push_back(pixels[index]);
    }
  }

  ColorIndex getPixel(uint16_t screenX, uint16_t screenY) {
    // translate from absolute to sprite relative coords
    uint16_t index = width * (screenY - top) + (screenX - left);
    return pixels[index];
  }

  void moveBy(int8_t x, int8_t y) {
    // TODO: add bounds checking
    this->left += x;
    this->right += x;

    this->top += y;
    this->bottom += y;
  }

  /**
   * @brief checks if x & y are inside of sprite
   *
   * @param x coord on screen
   * @param y coord on screen
   * @return true if pixel is inside of sprite
   * @return false otherwise
   */
  bool contains(uint8_t x, uint8_t y) {
    if (x >= left && x <= right) {
      if (y >= top && y <= bottom) {
        return true;
      }
    }
    return false;
  }
};

#endif  // GAME_SDK_SPRITE_H