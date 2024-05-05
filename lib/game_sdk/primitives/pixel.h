#ifndef GAME_SDK_PIXEL_H
#define GAME_SDK_PIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "primitives.h"

typedef struct {
  uint8_t x;
  uint8_t y;
  Color color;
} Pixel_t;

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_PIXEL_H