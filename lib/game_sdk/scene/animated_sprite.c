#include "animated_sprite.h"

#include <stdlib.h>

typedef struct AnimatedSprite_t {
  ColorIndex* pixels;
  Rectangle_t* bounds;

  /** @brief Number of frames which is animated, this number should be > 1 to
   * animate sprite */
  _u8 frames;
  _u8 currentFrame;

  _u16 indexesPerFrame;

  AnimationSpeed_t animationSpeed;
  _i8 updatesBeforeAnimation;
  bool isFrameChanged;
} AnimatedSprite_t;

AnimatedSprite_t* AnimatedSpriteCreate(const SpriteData_t* data,
                                       const AnimationSpeed_t animationSpeed) {
  AnimatedSprite_t* sprite =
      (AnimatedSprite_t*)malloc(sizeof(AnimatedSprite_t));

  sprite->pixels = data->indices;
  sprite->bounds = RectangleCreateWithSize(data->width, data->height, 0, 0);

  // we haave packed data as 2 indexes per byte, so we have always add div by 2
  sprite->indexesPerFrame = ((data->width) * (data->height)) / 2;
  sprite->frames = data->indicesCount / (sprite->indexesPerFrame);

  sprite->currentFrame = 0;

  sprite->animationSpeed = animationSpeed;

  // animation speed already contains count of updates required before switching
  // on next frame
  sprite->updatesBeforeAnimation = animationSpeed;

  // first draw
  sprite->isFrameChanged = true;

  return sprite;
}

void AnimatedSpriteDestroy(AnimatedSprite_t* sprite) {
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

  _u8 x = localPointForAnimatedSprite.x;
  _u8 y = localPointForAnimatedSprite.y;

  // we have 2 real pixels per item in "pixels" array we have to divide the
  // result of RectangleIndexOf() by 2
  _u16 indexInSprite = RectangleIndexOf(bounds, x, y) / 2;
  _u16 offset = (sprite->currentFrame * (sprite->indexesPerFrame));

  _u16 index = indexInSprite + offset;

  ColorIndexes indexes = sprite->pixels[index];

  bool isOdd = x & 1 == 1;
  if (isOdd) {
    return getSecondIndex(indexes);
  }
  return getFirstIndex(indexes);

  return 0;
}

Rectangle_t* AnimatedSpriteGetBounds(const AnimatedSprite_t* sprite) {
  return sprite->bounds;
}

void AnimatedSpriteUpdateState(AnimatedSprite_t* sprite) {
  sprite->updatesBeforeAnimation--;
  sprite->isFrameChanged = false;

  if (sprite->updatesBeforeAnimation < 0) {
    sprite->updatesBeforeAnimation = sprite->animationSpeed;
    sprite->isFrameChanged = true;
    sprite->currentFrame++;
    if (sprite->currentFrame >= sprite->frames) {
      sprite->currentFrame = 0;
    }
  }
}

bool AnimatedSpriteIsFrameChanged(AnimatedSprite_t* sprite) {
  return sprite->isFrameChanged;
}