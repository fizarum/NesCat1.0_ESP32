#include "animated_sprite.h"

#include <stdlib.h>

typedef struct AnimatedSprite_t {
  ColorIndex* pixels;
  Rectangle_t* bounds;

  /** @brief Number of actual images which is animated */
  uint8_t frames;
  uint8_t currentFrame;

  uint16_t pixelsPerFrame;

  AnimationSpeed_t animationSpeed;
} AnimatedSprite_t;

AnimatedSprite_t* AnimatedSpriteCreate(const uint8_t width,
                                       const uint8_t height,
                                       ColorIndex* const pixels,
                                       const uint16_t pixelsCount,
                                       const AnimationSpeed_t animationSpeed) {
  AnimatedSprite_t* sprite =
      (AnimatedSprite_t*)malloc(sizeof(AnimatedSprite_t));

  sprite->pixels = pixels;
  sprite->bounds = RectangleCreateWithSize(width, height, 0, 0);

  sprite->pixelsPerFrame = width * height;
  sprite->frames = pixelsCount / (sprite->pixelsPerFrame);
  sprite->currentFrame = 0;

  sprite->animationSpeed = animationSpeed;

  return sprite;
}

void AnimatedSpriteDestroy(AnimatedSprite_t* sprite) {
  free(sprite->pixels);
  RectangleDestroy(sprite->bounds);
  free(sprite);
}

Point_t localPointForAnimatedSprite;

ColorIndex AnimatedSpriteGetPixel(const AnimatedSprite_t* sprite,
                                  const uint16_t screenX,
                                  const uint16_t screenY,
                                  const ColorIndex fallback) {
  const Rectangle_t* bounds = sprite->bounds;

  if (RectangleConvertScreenCoordsToLocal(
          bounds, screenX, screenY, &localPointForAnimatedSprite) == false)
    return fallback;

  uint8_t x = localPointForAnimatedSprite.x;
  uint8_t y = localPointForAnimatedSprite.y;

  // we have 2 real pixels per item in "pixels" array we have to divide the
  // result of RectangleIndexOf() by 2
  uint16_t index = RectangleIndexOf(bounds, x, y) / 2;
  bool isOdd = x & 1 == 1;

  ColorIndexes indexes = sprite->pixels[index];
  if (isOdd) {
    return getSecondIndex(indexes);
  }
  return getFirstIndex(indexes);

  return 0;
}

void AnimatedSpriteShowNextFrame(AnimatedSprite_t* sprite) {
  sprite->currentFrame++;

  if (sprite->currentFrame > sprite->frames) {
    sprite->currentFrame = 0;
  }
}

Rectangle_t* AnimatedSpriteGetBounds(const AnimatedSprite_t* sprite) {
  return sprite->bounds;
}