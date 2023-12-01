#include "utils.h"

#include <Esp.h>
#include <Wire.h>
#include <log.h>

#define MAXFILENAME_LENGTH 64

/**
 * @brief Set (to 1) the bit in byte
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
uint8_t bit::setBit(uint8_t source, uint8_t position) {
  return source | (1UL << position);
}

/**
 * @brief Set or reset the bit in uint16_t, source value isn't mutated
 *
 * @param source source uint16_t
 * @param position of bit to change, counts from 0
 * @param isSet 0 means reset, > 0 - means set
 * @return updated copy of source value
 */
uint16_t bit::setBit16(uint16_t source, uint8_t position, bool isSet) {
  if (isSet == true) {
    return source | (1UL << position);
  } else {
    return source & (~(1UL << position));
  }
}

/**
 * @brief Reset (to 0) the bit in byte
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
uint8_t bit::resetBit(uint8_t source, uint8_t position) {
  // if position is 1 then it moves 0001 to 0010
  return source & (~(1UL << position));
}

bool bit::isBitSet(uint16_t source, uint8_t position) {
  uint16_t mask = (1UL << position);
  return (source & mask) == mask;
}

void strToLowerCase(char *str) {
  char *temp = str;
  while (*temp) {
    *temp = tolower(*temp);
    temp++;
  }
}

void getMemoryStatus() {
  debug("--------------------------------");
  debug("TOTAL HEAP: %u", ESP.getHeapSize());
  debug("FREE HEAP:: %u", ESP.getFreeHeap());
  uint32_t heapCapsFreeSize = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  debug("heap_caps_get_free_size: %u", heapCapsFreeSize);
  debug("--------------------------------");
}

// uint32_t size = ESP.getPsramSize();
void getPsRamStatus(uint32_t psramSize) {
  debug("--------------------------------");
  if (psramSize > 0) {
    debug("Total PSRAM: %u", ESP.getPsramSize());
    debug("Free PSRAM: %u", ESP.getFreePsram());
  } else {
    debug("NO PSRAM DETECTED.");
  }
  debug("--------------------------------");
}

uint8_t findI2CDevice(const uint8_t startAddress) {
  uint8_t address, response;
  for (address = startAddress; address < 127; address++) {
    Wire.beginTransmission(address);
    response = Wire.endTransmission();

    if (response == 0) {
      debug("found device on port: %x", address);
      return address;
    }
  }
  debug("device not found in i2c port");
  return 0;
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