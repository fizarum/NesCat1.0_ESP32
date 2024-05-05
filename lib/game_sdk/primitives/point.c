#include "point.h"

#include <stdlib.h>

Point_t *PointCreate(int16_t x, int16_t y) {
  Point_t *point = (Point_t *)malloc(sizeof(Point_t));
  point->x = x;
  point->y = y;
  return point;
}

void PointDestroy(Point_t *point) { free(point); }

void PointSet(Point_t *point, int16_t x, int16_t y) {
  point->x = x;
  point->y = y;
}

/** @brief reset point to start position (0,0) */
void PointReset(Point_t *point) {
  point->x = 0;
  point->y = 0;
}