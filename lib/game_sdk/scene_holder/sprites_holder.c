#include "sprites_holder.h"

#include <stdlib.h>

#include "../collections/array.h"

// max sprites count is 64
#define SPRITES_BACKGROUND_MAX_COUNT 28
#define SPRITES_FOREGROUIND_MAX_COUNT 30
#define SPRITES_HUD_MAX_COUNT 6

typedef struct SpritesHolder_t {
  Array_t *backgroundSprites;
  Array_t *foregroundSprites;
  Array_t *hudSprites;
  Array_t *backgroundAnimatedSprites;
  Array_t *foregroundAnimatedSprites;
  Array_t *hudAnimatedSprites;
} SpritesHolder_t;

Array_t *selectSpritesContainer(const SpritesHolder_t *holder,
                                const SpriteType_t containerType,
                                const bool isAnimated);
// Create standard sprite
Sprite_t *createSprite(SpritesHolder_t *holder, const _u8 width,
                       const _u8 height, const ColorIndexes *const pixels,
                       const size_t pixelsCount, const _u8 x, const _u8 y);

// Create animated sprite
AnimatedSprite_t *createAnimatedSprite(SpritesHolder_t *holder, const _u8 width,
                                       const _u8 height,
                                       const ColorIndex *const pixels,
                                       size_t pixelsCount,
                                       const AnimationSpeed_t animationSpeed,
                                       const _u8 x, const _u8 y);

SpritesHolder_t *SpritesHolderCreate() {
  SpritesHolder_t *holder = (SpritesHolder_t *)malloc(sizeof(SpritesHolder_t));

  holder->foregroundSprites = ArrayCreate(SPRITES_FOREGROUIND_MAX_COUNT);
  holder->backgroundSprites = ArrayCreate(SPRITES_BACKGROUND_MAX_COUNT);
  holder->hudSprites = ArrayCreate(SPRITES_HUD_MAX_COUNT);

  holder->foregroundAnimatedSprites =
      ArrayCreate(SPRITES_FOREGROUIND_MAX_COUNT);
  holder->backgroundAnimatedSprites = ArrayCreate(SPRITES_BACKGROUND_MAX_COUNT);
  holder->hudAnimatedSprites = ArrayCreate(SPRITES_HUD_MAX_COUNT);

  return holder;
}

void SpritesHolderDestroy(SpritesHolder_t *holder) {
  if (holder == NULL) return;

  ArrayDestroy(holder->backgroundSprites);
  ArrayDestroy(holder->foregroundSprites);
  ArrayDestroy(holder->hudSprites);

  ArrayDestroy(holder->backgroundAnimatedSprites);
  ArrayDestroy(holder->foregroundAnimatedSprites);
  ArrayDestroy(holder->hudAnimatedSprites);

  free(holder);
}

ObjectId SpritesHolderAddSprite(SpritesHolder_t *holder, const _u8 width,
                                const _u8 height,
                                const ColorIndexes *const pixels,
                                const size_t pixelsCount,
                                const SpriteType_t type, const _u8 x,
                                const _u8 y) {
  Sprite_t *sprite =
      createSprite(holder, width, height, pixels, pixelsCount, x, y);

  Array_t *spritesContainer = selectSpritesContainer(holder, type, false);
  bool isSpriteAdded = ArrayAdd(spritesContainer, sprite);

  if (isSpriteAdded == true) {
    return (ObjectId)sprite;
  }

  return OBJECT_ID_NA;
}

ObjectId SpritesHolderAddAnimatedSprite(SpritesHolder_t *holder, _u8 width,
                                        const _u8 height,
                                        const ColorIndexes *const pixels,
                                        const size_t pixelsCount,
                                        const SpriteType_t type,
                                        const AnimationSpeed_t animationSpeed,
                                        const _u8 x, const _u8 y) {
  AnimatedSprite_t *sprite = createAnimatedSprite(
      holder, width, height, pixels, pixelsCount, animationSpeed, x, y);

  Array_t *spritesContainer = selectSpritesContainer(holder, type, true);
  bool isSpriteAdded = ArrayAdd(spritesContainer, sprite);

  if (isSpriteAdded == true) {
    return (ObjectId)sprite;
  }

  return OBJECT_ID_NA;
}

ObjectId SpritesHolderGetSprite(const SpritesHolder_t *holder,
                                const SpriteType_t type, const bool isAnimated,
                                const _u8 x, const _u8 y) {
  Array_t *sprites = selectSpritesContainer(holder, type, isAnimated);

  _u16 size = ArraySize(sprites);
  void *sprite = NULL;
  Rectangle_t *bounds = NULL;

  for (_u16 index = 0; index < size; index++) {
    sprite = ArrayValueOf(sprites, index);
    if (isAnimated) {
      bounds = AnimatedSpriteGetBounds(sprite);
    } else {
      bounds = SpriteGetBounds(sprite);
    }
    if (RectangleContainsPoint(bounds, x, y)) {
      return (ObjectId)sprite;
    }
  }
  return OBJECT_ID_NA;
}

void SpritesHolderForeachAnimatedSprite(
    const SpritesHolder_t *holder, const SpriteType_t type,
    void (*foreach)(AnimatedSprite_t *sprite)) {
  Array_t *array = selectSpritesContainer(holder, type, true);
  _u16 size = ArraySize(array);

  for (_u16 index = 0; index < size; index++) {
    AnimatedSprite_t *sprite = ArrayValueOf(array, index);
    foreach (sprite);
  }
}

Array_t *selectSpritesContainer(const SpritesHolder_t *holder,
                                const SpriteType_t containerType,
                                const bool isAnimated) {
  switch (containerType) {
    case SPRITE_TYPE_HUD:
      return isAnimated ? holder->hudAnimatedSprites : holder->hudSprites;

    case SPRITE_TYPE_BACKGROUND:
      return isAnimated ? holder->backgroundAnimatedSprites
                        : holder->backgroundSprites;

    default:
      return isAnimated ? holder->foregroundAnimatedSprites
                        : holder->foregroundSprites;
  }
}

Sprite_t *createSprite(SpritesHolder_t *holder, const _u8 width,
                       const _u8 height, const ColorIndexes *const pixels,
                       const size_t pixelsCount, const _u8 x, const _u8 y) {
  Sprite_t *sprite = SpriteCreate(width, height, pixels, pixelsCount);
  if (x != 0 || y != 0) {
    SpriteMoveTo(sprite, x, y);
  }

  return sprite;
}

AnimatedSprite_t *createAnimatedSprite(SpritesHolder_t *holder, const _u8 width,
                                       const _u8 height,
                                       const ColorIndex *const pixels,
                                       size_t pixelsCount,
                                       const AnimationSpeed_t animationSpeed,
                                       const _u8 x, const _u8 y) {
  AnimatedSprite_t *sprite =
      AnimatedSpriteCreate(width, height, pixels, pixelsCount, animationSpeed);
  if (x != 0 || y != 0) {
    Rectangle_t *bounds = AnimatedSpriteGetBounds(sprite);
    RectangleMoveTo(bounds, x, y);
  }

  return sprite;
}