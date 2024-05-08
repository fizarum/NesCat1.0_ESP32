#include "sprite.h"

#include <stdlib.h>

#include "../primitives/rectangle.h"

typedef struct Sprite_t {
  ColorIndex* pixels;
  Rectangle_t* bounds;
} Sprite_t;

Sprite_t* SpriteCreate(const uint8_t width, const uint8_t height,
                       ColorIndex* const pixels, const uint16_t pixelsCount) {
  Sprite_t* sprite = (Sprite_t*)malloc(sizeof(Sprite_t));
  sprite->pixels = pixels;
  sprite->bounds = RectangleCreateWithSize(width, height, 0, 0);
  return sprite;
}

void SpriteDestroy(Sprite_t* sprite) {
  free(sprite->pixels);
  RectangleDestroy(sprite->bounds);
  free(sprite);
}

Point_t localPointForSprite;

ColorIndex SpriteGetPixel(const Sprite_t* sprite, const uint16_t screenX,
                          const uint16_t screenY, const ColorIndex fallback) {
  const Rectangle_t* bounds = sprite->bounds;

  if (RectangleConvertScreenCoordsToLocal(bounds, screenX, screenY,
                                          &localPointForSprite) == false)
    return fallback;

  // we have 2 real pixels per item in "pixels" array we have to divide the
  // result of RectangleIndexOf() by 2
  uint16_t index =
      RectangleIndexOf(bounds, localPointForSprite.x, localPointForSprite.y) /
      2;
  bool isOdd = localPointForSprite.x & 1 == 1;

  ColorIndexes indexes = sprite->pixels[index];
  if (isOdd) {
    return getSecondIndex(indexes);
  }
  return getFirstIndex(indexes);
}

Rectangle_t* SpriteGetBounds(const Sprite_t* sprite) { return sprite->bounds; }

bool SpriteContainsPoint(const Sprite_t* sprite, const uint8_t x,
                         const uint8_t y) {
  return RectangleContainsPoint(sprite->bounds, x, y);
}

void SpriteMoveTo(Sprite_t* sprite, const uint8_t x, const uint8_t y) {
  RectangleMoveTo(sprite->bounds, x, y);
}

#define MAX_FILENAME_SIZE 10

void SpriteMoveBy(Sprite_t* sprite, const uint8_t x, const uint8_t y) {
  RectangleMoveBy(sprite->bounds, x, y);
}

uint8_t SpriteGetVisibleLeftPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleLeftPosition(sprite->bounds);
}

uint8_t SpriteGetVisibleTopPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleTopPosition(sprite->bounds);
}

uint8_t SpriteGetVisibleRightPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleRightPosition(sprite->bounds);
}

uint8_t SpriteGetVisibleBottomPosition(const Sprite_t* sprite) {
  return RectangleGetVisibleBottomPosition(sprite->bounds);
}