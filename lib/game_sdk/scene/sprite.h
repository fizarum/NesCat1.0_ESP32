#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#include "../primitives/primitives.h"

typedef struct Sprite_t Sprite_t;
Sprite_t *SpriteCreate(const uint8_t width, const uint8_t height,
                       ColorIndex *const pixels, const uint16_t pixelsCount);

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
ColorIndex getPixel(const Sprite_t *sprite, const uint16_t screenX,
                    const uint16_t screenY, const ColorIndex fallback);

void SpriteMoveTo(Sprite_t *sprite, const uint8_t x, const uint8_t y);
void SpriteMoveBy(Sprite_t *sprite, const uint8_t x, const uint8_t y);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_H
