#ifndef GAME_SDK_PALETTE_H
#define GAME_SDK_PALETTE_H

#include <stdint.h>

#include <array>

#include "../device/display_device/palette.h"
#include "primitives.h"

#define PALETTE_SIZE 16

class Palette {
 private:
  std::array<Color, PALETTE_SIZE> palette = {};

  /**
   * @brief Index of color in palette which is used as background
   */
  ColorIndex background;

 public:
  Palette(ColorIndex background = 0) { this->background = background; }

  inline Color getColor(ColorIndex index) { return palette[index]; }
  inline Color getBackgroundColor() { return palette[background]; }
  inline ColorIndex getBackgroundColorIndex() { return background; }

  /**
   * @brief Helps to understand if color index should be drawn as an active
   * color
   *
   * @param index
   * @return true if color is visible - not background
   * @return false otherwise
   */
  inline bool isVisible(ColorIndex index) { return index != background; }
  inline void setColor(ColorIndex index, Color color) {
    palette[index] = color;
  }
};

#endif  // GAME_SDK_PALETTE_H