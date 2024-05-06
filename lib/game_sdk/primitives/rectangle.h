#ifndef GAME_SDK_RECTANGLE_H
#define GAME_SDK_RECTANGLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct Rectangle_t Rectangle_t;

Rectangle_t* RectangleCreate();
Rectangle_t* RectangleCreateWithSize(const uint8_t width, const uint8_t height,
                                     const uint8_t left, const uint8_t top);
void RectangleDestroy(Rectangle_t* rectangle);

uint8_t RectangleGetWith(const Rectangle_t* rectangle);
uint8_t RectangleGetHeight(const Rectangle_t* rectangle);
const int16_t RectangleGetLeftPosition(const Rectangle_t* rectangle);
const int16_t RectangleGetRightPosition(const Rectangle_t* rectangle);
const int16_t RectangleGetTopPosition(const Rectangle_t* rectangle);
const int16_t RectangleGetBottomPosition(const Rectangle_t* rectangle);

uint8_t RectangleGetVisibleLeftPosition(const Rectangle_t* rectangle);
uint8_t RectangleGetVisibleTopPosition(const Rectangle_t* rectangle);
uint8_t RectangleGetVisibleRightPosition(const Rectangle_t* rectangle);
uint8_t RectangleGetVisibleBottomPosition(const Rectangle_t* rectangle);

void RectangleReset(Rectangle_t* rectangle);
void RectangleResize(Rectangle_t* rectangle, const uint8_t width,
                     const uint8_t height);
void RectangleMoveBy(Rectangle_t* rectangle, const int8_t x, const int8_t y);
void RectangleMoveTo(Rectangle_t* rectangle, const uint8_t x, const uint8_t y);

bool RectangleContainsPoint(const Rectangle_t* rectangle, const uint8_t x,
                            const uint8_t y);

/**
 * @brief get index in 1 dimention array for specified x,y in rectangle
 *
 * @param x coordinate in rectangle
 * @param y coordinate in rectangle
 * @return index in 1d array
 */
uint32_t RectangleIndexOf(const Rectangle_t* rectangle, const uint8_t x,
                          const uint8_t y);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_RECTANGLE_H