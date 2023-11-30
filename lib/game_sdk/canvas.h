#ifndef GAME_SDK_CANVAS_H
#define GAME_SDK_CANVAS_H

#include <stdint.h>

#include <vector>

#include "configurator.h"
#include "palette.h"

/*
 * Virtual pixel size. Bigger pixel size - better performance
 * but smaller resolution
 */

// #define PIXEL_SIZE 2  // 160x120
// #define PIXEL_SIZE 3  // 106x80
// #define PIXEL_SIZE 4  // 80x60
// #define PIXEL_SIZE 5  // 64x48
// the best performance/pixel size value
#define PIXEL_SIZE 8  // 40x30
// #define PIXEL_SIZE 10  // 32x24
// #define PIXEL_SIZE 20  // 16x12
// #define PIXEL_SIZE 40  // 8x6

// display values
#define WIDTH_IN_V_PIXELS DISPLAY_WIDTH / PIXEL_SIZE
#define HEIGHT_IN_V_PIXELS DISPLAY_HEIGHT / PIXEL_SIZE

#define FRAME_BUFFER_SIZE WIDTH_IN_V_PIXELS* HEIGHT_IN_V_PIXELS

class Canvas {
 private:
  Palette* palette;

  // frame buffer of color indexes
  std::array<ColorIndex, FRAME_BUFFER_SIZE> pixels;

 public:
  Canvas(Color fillColor = 0);

  Color getPixel(size_t pixelIndex);
  void setPixel(size_t pixelIndex, ColorIndex colorIndex);
  size_t getPixelsCount();
};

#endif  // GAME_SDK_CANVAS_H