#ifndef GAME_SDK_SPRITE_DATA_H
#define GAME_SDK_SPRITE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "primitives.h"

typedef struct SpriteData_t {
  _u8 width;
  _u8 height;
  _u16 indicesCount;
  _ci *indices;
} SpriteData_t;

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_DATA_H