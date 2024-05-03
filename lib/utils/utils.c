#include "utils.h"

#include <ctype.h>

uint8_t setBit(uint8_t source, uint8_t position) {
  return source | (1UL << position);
}

uint16_t setBit16(uint16_t source, uint8_t position) {
  return source | (1UL << position);
}

uint32_t setBit32(uint32_t source, uint8_t position) {
  return source | (1UL << position);
}

uint8_t resetBit(uint8_t source, uint8_t position) {
  // if position is 1 then it moves 0001 to 0010
  return source & (~(1UL << position));
}

bool isBitSet(uint16_t source, uint8_t position) {
  uint16_t mask = (1UL << position);
  return (source & mask) == mask;
}

bool isBitSet32(uint32_t source, uint8_t position) {
  uint32_t mask = (1UL << position);
  return (source & mask) == mask;
}

void strToLowerCase(char *str) {
  char *temp = str;
  while (*temp) {
    *temp = tolower(*temp);
    temp++;
  }
}

uint32_t alignTo(uint32_t value, uint16_t blockSize) {
  uint32_t blockCounts = value / blockSize;
  uint32_t rounded = blockCounts * blockSize;
  if (value == rounded) {
    return rounded;
  }
  rounded += blockSize;
  return rounded;
}