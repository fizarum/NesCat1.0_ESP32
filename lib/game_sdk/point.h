#ifndef GAME_SDK_POINT_H
#define GAME_SDK_POINT_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

/**
 * @brief Structure of point in 2D space.
 *
 */
typedef struct {
  int16_t x;
  int16_t y;
} Point_t;

Point_t *PointCreate(int16_t x, int16_t y);
void PointDestroy(Point_t *point);
void PointSet(Point_t *point, int16_t x, int16_t y);
void PointReset(Point_t *point);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_POINT_H