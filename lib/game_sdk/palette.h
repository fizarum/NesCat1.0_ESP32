#ifndef GAME_SDK_PALETTE_H
#define GAME_SDK_PALETTE_H

#include <display_device/palette.h>
#include <stdint.h>

#include <array>

#define PALETTE_SIZE UINT8_MAX

#define COLOR_INDEX_UNDEF 255
#define COLOR_INDEX_TRANSPARENT 200
#define COLOR_INDEX_BACKGROUND 201

/**
 * @brief color in RGB565 format
 */
typedef uint16_t Color;

/**
 * @brief index of color in pallette
 */
typedef uint8_t ColorIndex;

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