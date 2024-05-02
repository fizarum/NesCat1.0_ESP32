#ifndef GAME_SDK_PALETTE_H
#define GAME_SDK_PALETTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../device/display_device/palette.h"
#include "primitives.h"

#define PALETTE_SIZE 16

typedef struct {
  Color colors[PALETTE_SIZE];

  /**
   * @brief Index of color in palette which is used as background
   */
  ColorIndex background;
} Palette_t;

Palette_t *PaletteCreate(ColorIndex background);
void PaletteDestroy(Palette_t *palette);

const Color PaletteGetColor(Palette_t *palette, ColorIndex index);
const Color PalettegetBackgroundColor(Palette_t *palette);
void PaletteSetColor(Palette_t *palette, ColorIndex index, Color color);

/**
 * @brief Helps to understand if color index should be drawn as an active
 * color
 *
 * @param index
 * @return true if color is visible - not background
 * @return false otherwise
 */
const bool PaletteIsColorVisible(Palette_t *palette, ColorIndex index);

#ifdef __cplusplus
}
#endif

#endif  // GAME_SDK_PALETTE_H