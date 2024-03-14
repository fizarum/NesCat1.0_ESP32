#ifndef GAME_SDK_PALETTE_H
#define GAME_SDK_PALETTE_H

#include <stdint.h>

#include <array>

#include "../device/display_device/palette.h"
#include "primitives.h"

#define PALETTE_SIZE UINT8_MAX

static inline bool ifColorIndexHasColor(ColorIndex colorIndex) {
  return colorIndex != COLOR_INDEX_UNDEF &&
         colorIndex != COLOR_INDEX_TRANSPARENT;
}

class Palette {
 private:
  std::array<Color, PALETTE_SIZE> palette = {};

 public:
  Palette(Color backgroundColor = COLOR_BLACK,
          Color transparentColor = COLOR_BLACK) {
    palette[COLOR_INDEX_BACKGROUND] = backgroundColor;
    palette[COLOR_INDEX_TRANSPARENT] = transparentColor;
  }

  inline Color getColor(ColorIndex index) { return palette[index]; }
  inline void setColor(ColorIndex index, Color color) {
    palette[index] = color;
  }
};

#endif  // GAME_SDK_PALETTE_H