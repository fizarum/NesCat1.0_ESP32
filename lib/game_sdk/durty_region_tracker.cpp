#include "durty_region_tracker.h"

void Line::setPixelOnLine(uint8_t pos) {
  if (pos < 32) {
    this->first = bit::setBit32(this->first, pos);
    return;
  }
  if (pos < 64) {
    this->second = bit::setBit32(this->second, pos);
    return;
  }
  if (pos < 96) {
    this->third = bit::setBit32(this->third, pos);
    return;
  }
  if (pos < 128) {
    this->fourth = bit::setBit32(this->fourth, pos);
    return;
  }

  this->fifth = bit::setBit32(this->fifth, pos);
}

bool Line::isPixelSetOnLine(uint8_t pos) {
  if (pos < 32) return bit::isBitSet32(this->first, pos);
  if (pos < 64) return bit::isBitSet32(this->second, pos);
  if (pos < 96) return bit::isBitSet32(this->third, pos);
  if (pos < 128) return bit::isBitSet32(this->fourth, pos);
  return bit::isBitSet32(this->fifth, pos);
}