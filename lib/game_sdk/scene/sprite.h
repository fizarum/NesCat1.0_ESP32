#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "../primitives/primitives.h"
#include "../primitives/rectangle.h"
#include "../primitives/sprite_data.h"

typedef struct Sprite_t Sprite_t;
Sprite_t *SpriteCreate(const SpriteData_t *data);

void SpriteDestroy(Sprite_t *sprite);

/**
 * @brief Get color index in sprite
 *
 * @param screenX - x coord on screen
 * @param screenY - y coord on screen
 * @param fallback - function will return this value if color index can not be
 * otained
 * @return ColorIndex
 */
_ci SpriteGetPixel(const Sprite_t *sprite, const _u16 screenX,
                   const _u16 screenY, const _ci fallback);

Rectangle_t *SpriteGetBounds(const Sprite_t *sprite);

bool SpriteContainsPoint(const Sprite_t *sprite, const _u8 x, const _u8 y);

_u8 SpriteGetVisibleLeftPosition(const Sprite_t *sprite);
_u8 SpriteGetVisibleTopPosition(const Sprite_t *sprite);
_u8 SpriteGetVisibleRightPosition(const Sprite_t *sprite);
_u8 SpriteGetVisibleBottomPosition(const Sprite_t *sprite);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_H
