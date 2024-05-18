#ifndef GAME_SDK_SPRITE_HOLDER_H
#define GAME_SDK_SPRITE_HOLDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "../types/simpler_types.h"
#include "primitives/primitives.h"
#include "primitives/rectangle.h"
#include "scene/animated_sprite.h"
#include "scene/sprite.h"

typedef enum {
  SPRITE_TYPE_BACKGROUND,
  SPRITE_TYPE_FOREGROUND,
  SPRITE_TYPE_HUD,
} SpriteType_t;

typedef struct SpritesHolder_t SpritesHolder_t;

SpritesHolder_t *SpritesHolderCreate();
void SpritesHolderDestroy(SpritesHolder_t *holder);

/**
 * @brief Creates Sprite_t instance and add to internal container
 *
 * @param holder
 * @param data
 * @param type
 * @param x
 * @param y
 * @return ObjectId unique id of sprite if it is added or OBJECT_ID_NA otherwise
 */
ObjectId SpritesHolderAddSprite(SpritesHolder_t *holder,
                                const SpriteData_t *data,
                                const SpriteType_t type, const _u8 x,
                                const _u8 y);

/**
 * @brief Creates AimatedSprite_t instance and add to internal container
 *
 * @param holder
 * @param data
 * @param type
 * @param x
 * @param y
 * @return ObjectId unique id of sprite if it is added or OBJECT_ID_NA otherwise
 */
ObjectId SpritesHolderAddAnimatedSprite(SpritesHolder_t *holder,
                                        const SpriteData_t *data,
                                        const SpriteType_t type,
                                        const AnimationSpeed_t animationSpeed,
                                        const _u8 x, const _u8 y);

/**
 * @brief Get Sprite object by [x,y] coordinates
 *
 *
 * @param holder
 * @param type
 * @param isAnimated
 * @param x
 * @param y
 * @return ObjectId of sprite (can be casted to Sprite_t or AnimatedSprite_t) or
 * OBJECT_ID_NA if sprite cant be found
 */
ObjectId SpritesHolderGetSprite(const SpritesHolder_t *holder,
                                const SpriteType_t type, const bool isAnimated,
                                const _u8 x, const _u8 y);

void SpritesHolderForeachAnimatedSprite(
    const SpritesHolder_t *holder, const SpriteType_t type,
    void (*foreach)(AnimatedSprite_t *sprite));

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_SPRITE_HOLDER_H