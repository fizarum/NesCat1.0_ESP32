#include "sprite.h"

#include <stdlib.h>

#include "../primitives/rectangle.h"

typedef struct {
  ColorIndex* pixels;
  Rectangle_t* bounds;
} SpriteImpl_t;

Sprite_t* SpriteCreate(const uint8_t width, const uint8_t height,
                       ColorIndex* const pixels, const uint16_t pixelsCount) {
  SpriteImpl_t* impl = malloc(sizeof(SpriteImpl_t));
  impl->pixels = pixels;
  impl->bounds = RectangleCreateWithSize(width, height, 0, 0);
  return (Sprite_t*)impl;
}

void SpriteDestroy(Sprite_t* sprite) {
  SpriteImpl_t* impl = (SpriteImpl_t*)sprite;
  free(impl->pixels);
  RectangleDestroy(impl->bounds);
  free(impl);
}

ColorIndex getPixel(const Sprite_t* sprite, const uint16_t screenX,
                    const uint16_t screenY, const ColorIndex fallback) {
  SpriteImpl_t* impl = (SpriteImpl_t*)sprite;
  const Rectangle_t* bounds = impl->bounds;

  if (RectangleContainsPoint(bounds, screenX, screenY) == false)
    return fallback;

  // translate from absolute to sprite relative coords
  uint8_t x = screenX - RectangleGetLeftPosition(bounds);
  uint8_t y = screenY - RectangleGetTopPosition(bounds);

  // we have 2 real pixels per item in "pixels" array we have to divide the
  // result of RectangleIndexOf() by 2
  uint16_t index = RectangleIndexOf(bounds, x, y) / 2;
  bool isOdd = x & 1 == 1;

  ColorIndexes indexes = impl->pixels[index];
  if (isOdd) {
    return getSecondIndex(indexes);
  }
  return getFirstIndex(indexes);
}

void SpriteMoveTo(Sprite_t* sprite, const uint8_t x, const uint8_t y) {
  SpriteImpl_t* impl = (SpriteImpl_t*)sprite;

  RectangleMoveTo(impl->bounds, x, y);
}

#define MAX_FILENAME_SIZE 10

void SpriteMoveBy(Sprite_t* sprite, const uint8_t x, const uint8_t y) {
  SpriteImpl_t* impl = (SpriteImpl_t*)sprite;

  RectangleMoveBy(impl->bounds, x, y);
}