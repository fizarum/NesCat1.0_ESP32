#ifndef GAME_SDK_POINT_H
#define GAME_SDK_POINT_H

#include <stdint.h>
/**
 * @brief Class of point in 2D space.
 *
 */
class Point {
 private:
  int16_t x;
  int16_t y;

 public:
  Point() : x(0), y(0) {}
  Point(int16_t x, int16_t y) : x(x), y(y) {}

  int16_t getX() { return x; }
  int16_t getY() { return y; }

  void set(int16_t x, int16_t y) {
    this->x = x;
    this->y = y;
  }

  void setX(int16_t x) { this->x = x; }
  void setY(int16_t y) { this->y = y; }

  /**
   * @brief reset point to start position (0,0)
   *
   */
  void reset() {
    this->x = 0;
    this->y = 0;
  }
};

#endif  // GAME_SDK_POINT_H