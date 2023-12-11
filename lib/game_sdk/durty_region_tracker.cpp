#include "durty_region_tracker.h"

void setPixelOnLine(Line *line, uint8_t pos) {
  if (pos < 32) {
    line->first = bit::setBit32(line->first, pos);
    return;
  }
  if (pos < 64) {
    line->second = bit::setBit32(line->second, pos);
    return;
  }
  if (pos < 96) {
    line->third = bit::setBit32(line->third, pos);
    return;
  }
  if (pos < 128) {
    line->fourth = bit::setBit32(line->fourth, pos);
    return;
  }

  line->fifth = bit::setBit32(line->fifth, pos);
}

bool isPixelSetOnLine(Line *line, uint8_t pos) {
  if (pos < 32) return bit::isBitSet32(line->first, pos);
  if (pos < 64) return bit::isBitSet32(line->second, pos);
  if (pos < 96) return bit::isBitSet32(line->third, pos);
  if (pos < 128) return bit::isBitSet32(line->fourth, pos);
  return bit::isBitSet32(line->fifth, pos);
}