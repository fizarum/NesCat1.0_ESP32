#ifndef GAME_SDK_ANIMATED_SPRITE_H
#define GAME_SDK_ANIMATED_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "../primitives/primitives.h"
#include "../primitives/rectangle.h"

typedef enum {
  ANIMATION_SPEED_SUPER_SLOW,
  ANIMATION_SPEED_SLOW,
  ANIMATION_SPEED_NORMAL,
  ANIMATION_SPEED_FAST,
  ANIMATION_SPEED_SUPER_FAST
} AnimationSpeed_t;

typedef struct AnimatedSprite_t AnimatedSprite_t;

AnimatedSprite_t *AnimatedSpriteCreate(const uint8_t width,
                                       const uint8_t height,
                                       ColorIndex *const pixels,
                                       const uint16_t pixelsCount,
                                       const AnimationSpeed_t animationSpeed);

void AnimatedSpriteDestroy(AnimatedSprite_t *sprite);

ColorIndex AnimatedSpriteGetPixel(const AnimatedSprite_t *sprite,
                                  const uint16_t screenX,
                                  const uint16_t screenY,
                                  const ColorIndex fallback);

Rectangle_t *AnimatedSpriteGetBounds(const AnimatedSprite_t *sprite);

void AnimatedSpriteShowNextFrame(AnimatedSprite_t *sprite);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_ANIMATED_SPRITE_H