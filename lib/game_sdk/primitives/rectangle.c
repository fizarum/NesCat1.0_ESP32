#include "rectangle.h"

#include <stdlib.h>

#include "sdk_configurator.h"

typedef struct Rectangle_t {
  int16_t left;
  int16_t top;
  int16_t right;
  int16_t bottom;
  uint8_t width;
  uint8_t height;
} Rectangle_t;

Rectangle_t* RectangleCreate() {
  Rectangle_t* rectangle = (Rectangle_t*)malloc(sizeof(Rectangle_t));
  RectangleReset(rectangle);
  return rectangle;
}

Rectangle_t* RectangleCreateWithSize(const uint8_t width, const uint8_t height,
                                     const uint8_t left, const uint8_t top) {
  Rectangle_t* rectangle = (Rectangle_t*)malloc(sizeof(Rectangle_t));
  rectangle->width = width;
  rectangle->height = height;

  rectangle->left = left;
  rectangle->top = top;
  rectangle->right = left + width - 1;
  rectangle->bottom = top + height - 1;

  return rectangle;
}

void RectangleDestroy(Rectangle_t* rectangle) { free(rectangle); }

uint8_t RectangleGetWith(const Rectangle_t* rectangle) {
  return rectangle->width;
}

uint8_t RectangleGetHeight(const Rectangle_t* rectangle) {
  return rectangle->height;
}

const int16_t RectangleGetLeftPosition(const Rectangle_t* rectangle) {
  return rectangle->left;
}
const int16_t RectangleGetRightPosition(const Rectangle_t* rectangle) {
  return rectangle->right;
}

const int16_t RectangleGetTopPosition(const Rectangle_t* rectangle) {
  return rectangle->top;
}

const int16_t RectangleGetBottomPosition(const Rectangle_t* rectangle) {
  return rectangle->bottom;
}

uint8_t RectangleGetVisibleLeftPosition(const Rectangle_t* rectangle) {
  return rectangle->left < 0 ? 0 : rectangle->left;
}

uint8_t RectangleGetVisibleTopPosition(const Rectangle_t* rectangle) {
  return rectangle->top < 0 ? 0 : rectangle->top;
}

uint8_t RectangleGetVisibleRightPosition(const Rectangle_t* rectangle) {
  uint8_t rightPos = rectangle->right;

  // if object is out of screen
  if (rightPos < 0) return 0;
  return rightPos >= WIDTH_IN_V_PIXELS ? WIDTH_IN_V_PIXELS - 1 : rightPos;
}

uint8_t RectangleGetVisibleBottomPosition(const Rectangle_t* rectangle) {
  uint8_t bottomPos = rectangle->bottom;

  // if object is out of screen
  if (bottomPos < 0) return 0;
  return bottomPos >= HEIGHT_IN_V_PIXELS ? HEIGHT_IN_V_PIXELS - 1 : bottomPos;
}

void RectangleReset(Rectangle_t* rectangle) {
  rectangle->width = 0;
  rectangle->height = 0;
  rectangle->left = 0;
  rectangle->top = 0;
  rectangle->right = 0;
  rectangle->bottom = 0;
}

void RectangleResize(Rectangle_t* rectangle, const uint8_t width,
                     const uint8_t height) {
  rectangle->right = rectangle->left + width - 1;
  rectangle->bottom = rectangle->top + height - 1;
  rectangle->width = width;
  rectangle->height = height;
}

void RectangleMoveBy(Rectangle_t* rectangle, const int8_t x, const int8_t y) {
  rectangle->left += x;
  rectangle->right += x;
  rectangle->top += y;
  rectangle->bottom += y;
}

void RectangleMoveTo(Rectangle_t* rectangle, const uint8_t x, const uint8_t y) {
  rectangle->left = x;
  rectangle->top = y;
  rectangle->right = rectangle->left + rectangle->width - 1;
  rectangle->bottom = rectangle->top + rectangle->height - 1;
}

bool RectangleContainsPoint(const Rectangle_t* rectangle, const uint8_t x,
                            const uint8_t y) {
  return (x >= rectangle->left && x <= rectangle->right) &&
         (y >= rectangle->top && y <= rectangle->bottom);
}

bool RectangleConvertScreenCoordsToLocal(const Rectangle_t* rectangle,
                                         const uint16_t screenX,
                                         const uint16_t screenY,
                                         Point_t* localPoint) {
  bool rectangleHasAPoint = RectangleContainsPoint(rectangle, screenX, screenY);
  if (rectangleHasAPoint == true) {
    // translate from absolute to sprite relative coords
    uint8_t x = screenX - RectangleGetLeftPosition(rectangle);
    uint8_t y = screenY - RectangleGetTopPosition(rectangle);

    // and save results to localPoint
    localPoint->x = x;
    localPoint->y = y;
  }
  return rectangleHasAPoint;
}

uint32_t RectangleIndexOf(const Rectangle_t* rectangle, const uint8_t x,
                          const uint8_t y) {
  return y * rectangle->width + x;
}