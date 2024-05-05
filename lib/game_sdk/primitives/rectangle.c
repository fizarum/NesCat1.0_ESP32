#include "rectangle.h"

#include <stdlib.h>

typedef struct {
  int16_t left;
  int16_t top;
  int16_t right;
  int16_t bottom;
  uint8_t width;
  uint8_t height;
} RectangleImpl_t;

Rectangle_t* RectangleCreate() {
  RectangleImpl_t* rectangle = malloc(sizeof(RectangleImpl_t));
  RectangleReset((Rectangle_t*)rectangle);
  return (Rectangle_t*)rectangle;
}

Rectangle_t* RectangleCreateWithSize(const uint8_t width, const uint8_t height,
                                     const uint8_t left, const uint8_t top) {
  RectangleImpl_t* rectangle = malloc(sizeof(RectangleImpl_t));
  rectangle->width = width;
  rectangle->height = height;

  rectangle->left = left;
  rectangle->top = top;
  rectangle->right = left + width - 1;
  rectangle->bottom = top + height - 1;

  return (Rectangle_t*)rectangle;
}

void RectangleDestroy(Rectangle_t* rectangle) { free(rectangle); }

uint8_t RectangleGetWith(const Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return impl->width;
}

uint8_t RectangleGetHeight(const Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return impl->height;
}

const int16_t RectangleGetLeftPosition(const Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return impl->left;
}
const int16_t RectangleGetRightPosition(const Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return impl->right;
}

const int16_t RectangleGetTopPosition(const Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return impl->top;
}

const int16_t RectangleGetBottomPosition(const Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return impl->bottom;
}

void RectangleReset(Rectangle_t* rectangle) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  impl->width = 0;
  impl->height = 0;
  impl->left = 0;
  impl->top = 0;
  impl->right = 0;
  impl->bottom = 0;
}

void RectangleResize(Rectangle_t* rectangle, const uint8_t width,
                     const uint8_t height) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  impl->right = impl->left + width - 1;
  impl->bottom = impl->top + height - 1;
  impl->width = width;
  impl->height = height;
}

void RectangleMoveBy(Rectangle_t* rectangle, const int8_t x, const int8_t y) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  impl->left += x;
  impl->right += x;
  impl->top += y;
  impl->bottom += y;
}

void RectangleMoveTo(Rectangle_t* rectangle, const uint8_t x, const uint8_t y) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;

  impl->left = x;
  impl->top = y;
  impl->right = impl->left + impl->width - 1;
  impl->bottom = impl->top + impl->height - 1;
}

bool RectangleContainsPoint(const Rectangle_t* rectangle, const uint8_t x,
                            const uint8_t y) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;
  return (x >= impl->left && x <= impl->right) &&
         (y >= impl->top && y <= impl->bottom);
}

uint32_t RectangleIndexOf(const Rectangle_t* rectangle, const uint8_t x,
                          const uint8_t y) {
  RectangleImpl_t* impl = (RectangleImpl_t*)rectangle;

  return y * impl->width + x;
}