#ifndef GAME_SDK_SPRITE_H
#define GAME_SDK_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "../primitives/primitives.h"
#include "../primitives/rectangle.h"

typedef struct Sprite_t Sprite_t;
Sprite_t *SpriteCreate(const uint8_t width, const uint8_t height,
                       const ColorIndex *const pixels,
                       const uint16_t pixelsCount);

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
ColorIndex SpriteGetPixel(const Sprite_t *sprite, const uint16_t screenX,
                          const uint16_t screenY, const ColorIndex fallback);

Rectangle_t *SpriteGetBounds(const Sprite_t *sprite);

bool SpriteContainsPoint(const Sprite_t *sprite, const uint8_t x,
                         const uint8_t y);
// TODO: remove, should be done through Rectangle API
void SpriteMoveTo(Sprite_t *sprite, const uint8_t x, const uint8_t y);
void SpriteMoveBy(Sprite_t *sprite, const uint8_t x, const uint8_t y);

uint8_t SpriteGetVisibleLeftPosition(const Sprite_t *sprite);
uint8_t SpriteGetVisibleTopPosition(const Sprite_t *sprite);
uint8_t SpriteGetVisibleRightPosition(const Sprite_t *sprite);
uint8_t SpriteGetVisibleBottomPosition(const Sprite_t *sprite);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_H
