#ifndef GAME_SDK_PRIMITIVES_H
#define GAME_SDK_PRIMITIVES_H

#include <stdint.h>

typedef uint16_t ObjectId;
#define OBJECT_ID_UNDEF UINT16_MAX

/**
 * @brief color in RGB565 format
 */
typedef uint16_t Color;

/**
 * @brief index of color in pallette
 */
typedef uint8_t ColorIndex;

/**
 * @brief Direction can have such values:
 *        1 2 3
 *        4 0 5
 *        6 7 8
 * where 0: no direction, 1: left-top, 8: right-bottom, etc.
 */
typedef uint8_t Direction;
#define DIRECTION_LT 1
#define DIRECTION_T 2
#define DIRECTION_RT 3
#define DIRECTION_L 4
#define DIRECTION_NONE 0
#define DIRECTION_R 5
#define DIRECTION_LB 6
#define DIRECTION_B 7
#define DIRECTION_RB 8

/**
 * @brief Get the Direction by x, y coords. All calculations are made
 * for starting point at (0,0). Check Direction definitions for details
 * @param x - offset value by x axis
 * @param y - offset value by y axis
 * @return direction value
 */
const static inline uint8_t getDirection(int8_t x, int8_t y) {
  if (x == 0 && y == 0) return DIRECTION_NONE;
  if (x < 0 && y < 0) return DIRECTION_LT;
  if (x == 0 && y < 0) return DIRECTION_T;
  if (x > 0 && y < 0) return DIRECTION_RT;
  if (x < 0 && y == 0) return DIRECTION_L;
  if (x > 0 && y == 0) return DIRECTION_R;
  if (x < 0 && y > 0) return DIRECTION_LB;
  if (x == 0 && y > 0) return DIRECTION_B;
  if (x > 0 && y > 0) return DIRECTION_RB;
}

#endif  // GAME_SDK_PRIMITIVES_H