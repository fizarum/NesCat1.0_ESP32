#include "canvas.h"

void setupDefaultPalette(Palette *palette);

Canvas::Canvas(Color fillColor) {
  this->pixels.fill(fillColor);

  this->palette = new Palette();
  setupDefaultPalette(this->palette);
}

Color Canvas::getPixel(size_t pixelIndex) {
  return palette->getColor(pixels[pixelIndex]);
}

void Canvas::setPixel(size_t pixelIndex, ColorIndex colorIndex) {
  pixels[pixelIndex] = colorIndex;
}

size_t Canvas::getPixelsCount() { return pixels.size(); }

void setupDefaultPalette(Palette *palette) {
  palette->setColor(0, COLOR_BLACK);
  palette->setColor(1, COLOR_PEARL);
  palette->setColor(2, COLOR_WATERMELON_RED);
  palette->setColor(3, COLOR_PEWTER_BLUE);
  palette->setColor(4, COLOR_PURPLE_TAUPE);
  palette->setColor(5, COLOR_FOREST_GREEN);
  palette->setColor(6, COLOR_INDIGO);
  palette->setColor(7, COLOR_SUNRAY);
  palette->setColor(8, COLOR_LIGHT_TAUPE);
  palette->setColor(9, COLOR_FELDGRAU);
  palette->setColor(10, COLOR_CEDAR_CHEST);
  palette->setColor(11, COLOR_DARK_CHARCOAL);
  palette->setColor(12, COLOR_SONIC_SILVER);
  palette->setColor(13, COLOR_ASPARAGUS);
  palette->setColor(14, COLOR_SEA_SERPENT);
  palette->setColor(15, COLOR_GRAY);
}