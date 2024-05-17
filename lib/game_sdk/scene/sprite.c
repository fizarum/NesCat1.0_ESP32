#include "sprite.h"

#include <stdlib.h>

#include "../primitives/rectangle.h"

typedef struct Sprite_t {
  _ci* pixels;
  Rectangle_t* bounds;
} Sprite_t;

Sprite_t* SpriteCreate(const _u8 width, const _u8 height,
                       const _ci* const pixels, const _u16 pixelsCount) {
  Sprite_t* sprite = (Sprite_t*)malloc(sizeof(Sprite_t));
  sprite->pixels = (_ci* const)pixels;
  sprite->bounds = RectangleCreateWithSize(width, height, 0, 0);
  return sprite;
}

void SpriteDestroy(Sprite_t* sprite) {
  free(sprite->pixels);
  RectangleDestroy(sprite->bounds);
  free(sprite);
}

Point_t __localPoint;

_ci SpriteGetPixel(const Sprite_t* sprite, const _u16 screenX,
                   const _u16 screenY, const _ci fallback) {
  const Rectangle_t* bounds = sprite->bounds;

  if (RectangleConvertScreenCoordsToLocal(bounds, screenX, screenY,
                                          &__localPoint) == false)
    return fallback;

  // we have 2 real pixels per item in "pixels" array we have to divide the
  // result of RectangleIndexOf() by 2
  _u16 index = RectangleIndexOf(bounds, __localPoint.x, __localPoint.y) / 2;
  bool isOdd = __localPoint.x & 1 == 1;

  ColorIndexes indexes = sprite->pixels[index];
  if (isOdd) {
    return getSecondIndex(indexes);
  }
  return getFirstIndex(indexes);
}

Rectangle_t* SpriteGetBounds(const Sprite_t* sprite) { return sprite->bounds; }

bool SpriteContainsPoint(const Sprite_t* sprite, const _u8 x, const _u8 y) {
  return RectangleContainsPoint(sprite->bounds, x, y);
}

_u8 SpriteGetVisibleLeftPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleLeftPosition(sprite->bounds);
}

_u8 SpriteGetVisibleTopPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleTopPosition(sprite->bounds);
}

_u8 SpriteGetVisibleRightPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleRightPosition(sprite->bounds);
}

_u8 SpriteGetVisibleBottomPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleBottomPosition(sprite->bounds);
}