#ifndef GAME_SDK_PRIMITIVES_H
#define GAME_SDK_PRIMITIVES_H

#include "../types/simpler_types.h"

typedef _ptr ObjectId;

#define OBJECT_ID_NA 0

/**
 * @brief color in RGB565 format
 */
typedef uint16_t Color;

/**
 * @brief index of color in pallette, can have values in range: 0..15
 */
typedef uint8_t ColorIndex;
typedef ColorIndex _ci;

/**
 * @brief pair of ColorIndex values first 4 bits represents value of first
 * ColorIndex the second 4 - second ColorIndex.
 * Can have value in range: 0..255
 */
typedef uint8_t ColorIndexes;

/**
 * @brief Create a Color Index object from two palette index (pixels)
 *
 * @param firstIndex palette index [0..15] for first pixel
 * @param secondIndex palette index [0..15] for second pixel
 * @return const ColorIndex
 */
const static ColorIndexes createColorIndex(ColorIndex firstIndex,
                                           ColorIndex secondIndex) {
  ColorIndexes indexes = 0;
  indexes = firstIndex << 4;
  indexes |= secondIndex;

  return indexes;
}

/**
 * @brief Get the First ColorIndex from ColorIndexes
 *
 * @param indexes index which have 2 indexes
 * @return first index from ColorIndexes
 */
const static inline ColorIndex getFirstIndex(ColorIndexes indexes) {
  return indexes >> 4;
}

/**
 * @brief Get the Second ColorIndex from ColorIndexes
 *
 * @param indexes index which have 2 indexes
 * @return second index from ColorIndexes
 */
const static inline ColorIndex getSecondIndex(ColorIndexes indexes) {
  // return values which are ok with 0b0001111 mask and '&' operator
  return indexes & 0xf;
}

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
const static inline Direction getDirection(int8_t x, int8_t y) {
  if (x < 0 && y < 0) return DIRECTION_LT;
  if (x == 0 && y < 0) return DIRECTION_T;
  if (x > 0 && y < 0) return DIRECTION_RT;
  if (x < 0 && y == 0) return DIRECTION_L;
  if (x > 0 && y == 0) return DIRECTION_R;
  if (x < 0 && y > 0) return DIRECTION_LB;
  if (x == 0 && y > 0) return DIRECTION_B;
  if (x > 0 && y > 0) return DIRECTION_RB;
  return DIRECTION_NONE;
}

#endif  // GAME_SDK_PRIMITIVES_H