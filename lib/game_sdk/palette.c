#include "palette.h"

#include <stdlib.h>

Palette_t *PaletteCreate(ColorIndex background) {
  Palette_t *palette = (Palette_t *)malloc(sizeof(Palette_t));
  palette->background = background;
  return palette;
}

void PaletteDestroy(Palette_t *palette) { free(palette); }

const Color PaletteGetColor(Palette_t *palette, ColorIndex index) {
  return palette->colors[index];
}

const Color PalettegetBackgroundColor(Palette_t *palette) {
  return palette->colors[palette->background];
}

void PaletteSetColor(Palette_t *palette, ColorIndex index, Color color) {
  palette->colors[index] = color;
}

const bool PaletteIsColorVisible(Palette_t *palette, ColorIndex index) {
  return index != palette->background;
}