#ifndef GAME_SDK_ANIMATED_SPRITE_H
#define GAME_SDK_ANIMATED_SPRITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "../primitives/primitives.h"
#include "../primitives/rectangle.h"
#include "../primitives/sprite_data.h"

/**
 * @brief Animation speed calculated from formula 1000 millis / 20 updates.
 * So every iteration (50 millis) is a minimal fraction of animation time
 * Thus, for super fast speed its 5, means 5 * 50 millis = 250 millis for one
 * frame or 4 fps
 * speed normal then is: 20 * 50 = 1000 millis or 1 fps, and so on...
 * This is still valid for faster screen updates (for example 30, fraction: 33
 * millis), since its close to first statement
 */
typedef enum {
  ANIMATION_SPEED_SUPER_SLOW = 40,
  ANIMATION_SPEED_SLOW = 30,
  ANIMATION_SPEED_NORMAL = 20,
  ANIMATION_SPEED_FAST = 10,
  ANIMATION_SPEED_SUPER_FAST = 5
} AnimationSpeed_t;

typedef struct AnimatedSprite_t AnimatedSprite_t;

AnimatedSprite_t *AnimatedSpriteCreate(const SpriteData_t *data,
                                       const AnimationSpeed_t animationSpeed);

void AnimatedSpriteDestroy(AnimatedSprite_t *sprite);

/**
 * @brief Get color index in animated sprite
 *
 * @param screenX - x coord on screen
 * @param screenY - y coord on screen
 * @param fallback - function will return this value if color index can not be
 * otained
 * @return ColorIndex
 */
ColorIndex AnimatedSpriteGetPixel(const AnimatedSprite_t *sprite,
                                  const uint16_t screenX,
                                  const uint16_t screenY,
                                  const ColorIndex fallback);

Rectangle_t *AnimatedSpriteGetBounds(const AnimatedSprite_t *sprite);

void AnimatedSpriteUpdateState(AnimatedSprite_t *sprite);

bool AnimatedSpriteIsFrameChanged(AnimatedSprite_t *sprite);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_ANIMATED_SPRITE_H