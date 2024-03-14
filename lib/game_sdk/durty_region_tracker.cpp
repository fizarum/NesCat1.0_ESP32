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

void Line::resetLine() {
  this->first = 0;
  this->second = 0;
  this->third = 0;
  this->fourth = 0;
  this->fifth = 0;
}

#ifdef PORT_SDK
const uint8_t _buff_len = 200;
char buff[_buff_len];

void Line::printLine(char durtyPixel) {
  memset(buff, 0, _buff_len);
  for (uint8_t pos = 0; pos <= _buff_len; ++pos) {
    buff[pos] = this->isPixelSetOnLine(pos) == true ? durtyPixel : ' ';
  }
  printf("%s\n", buff);
}

#endif  // PORT_SDK