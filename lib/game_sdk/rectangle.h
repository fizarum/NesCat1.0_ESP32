#ifndef GAME_SDK_RECTANGLE_H
#define GAME_SDK_RECTANGLE_H

#include <stdint.h>

#include "sdk_configurator.h"

class Rectangle {
 private:
  int16_t left;
  int16_t top;
  int16_t right;
  int16_t bottom;
  uint8_t width;
  uint8_t height;

 public:
  Rectangle() : Rectangle(0, 0, 0, 0) {}

  Rectangle(uint8_t width, uint8_t height, uint8_t left = 0, uint8_t top = 0) {
    this->left = left;
    this->top = top;
    this->right = left + width - 1;
    this->bottom = top + height - 1;
    this->width = width;
    this->height = height;
  }

  Rectangle(Rectangle* r) {
    this->left = r->left;
    this->top = r->top;
    this->right = left + r->width - 1;
    this->bottom = top + r->height - 1;
    this->width = r->width;
    this->height = r->height;
  }

  uint8_t getWidth() { return width; }
  uint8_t getHeight() { return height; }
  int16_t getLeft() { return left; }
  int16_t getTop() { return top; }
  int16_t getRight() { return right; }
  int16_t getBottom() { return bottom; }

  uint8_t getVisibleLeft() { return left < 0 ? 0 : left; }
  uint8_t getVisibleTop() { return top < 0 ? 0 : top; }
  uint8_t getVisibleRight() {
    if (right < 0) return 0;
    if (right >= WIDTH_IN_V_PIXELS) return WIDTH_IN_V_PIXELS - 1;

    return right;
  }

  uint8_t getVisibleBottom() {
    if (bottom < 0) return 0;
    if (bottom >= HEIGHT_IN_V_PIXELS) return HEIGHT_IN_V_PIXELS - 1;

    return bottom;
  }

  void set(int16_t left, int16_t top, int16_t right, int16_t bottom) {
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
    this->width = right - left + 1;
    this->height = bottom - top + 1;
  }

  void resize(uint8_t width, uint8_t height) {
    this->right = left + width - 1;
    this->bottom = top + height - 1;
    this->width = width;
    this->height = height;
  }

  void moveBy(int8_t x, int8_t y) {
    this->left += x;
    this->right += x;

    this->top += y;
    this->bottom += y;
  }

  void moveTo(uint8_t x, uint8_t y) {
    this->left = x;
    this->top = y;
    this->right = left + width - 1;
    this->bottom = top + height - 1;
  }

  /**
   * @brief checks if x & y are inside of rectangle
   *
   * @param x coord on screen
   * @param y coord on screen
   * @return true if pixel is inside of rectangle
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

  bool hasSize(uint8_t width, uint8_t height) {
    return this->width == width && this->height == height;
  }

  bool isEmpty() { return width == 0 && height == 0; }

  /**
   * @brief get index in 1 dimention array for specified x,y in rectangle
   *
   * @param x coordinate in rectangle
   * @param y coordinate in rectangle
   * @return index in 1d array
   */
  uint32_t indexOf(uint8_t x, uint8_t y) { return y * this->width + x; }
};

#endif  // GAME_SDK_RECTANGLE_H