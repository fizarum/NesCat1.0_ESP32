#ifndef GAME_SDK_PALETTE_H
#define GAME_SDK_PALETTE_H

#include <display_device/palette.h>
#include <stdint.h>

#include <array>

#define PALETTE_SIZE 32

/**
 * @brief color in RGB565 format
 */
typedef uint16_t Color;

/**
 * @brief index of color in pallette
 */
typedef uint8_t ColorIndex;

class Palette {
 private:
  std::array<Color, PALETTE_SIZE> palette = {};

 public:
  Palette() { palette.fill(COLOR_BLACK); }
  inline Color getColor(ColorIndex index) { return palette[index]; }
  inline void setColor(ColorIndex index, Color color) {
    palette[index] = color;
  }
};

#endif  // GAME_SDK_PALETTE_H