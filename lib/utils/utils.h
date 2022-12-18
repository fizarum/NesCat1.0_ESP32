#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void debug(const char *message);
void debug(const char *templateString, uint32_t value);
void debug(const char *templateString, const char *value);
void debug(const char *templateString, const void *value);

namespace bit {
uint8_t setBit(uint8_t source, uint8_t position);
uint8_t resetBit(uint8_t source, uint8_t position);
uint16_t setBit16(uint16_t source, uint8_t position, uint8_t isSet = 1);
bool isBitSet(uint16_t source, uint8_t position);
}  // namespace bit

/**
 * @brief modify current string by changing all
 * letters to lower case
 *
 * @param string
 */
void toLowerCase(char *string);

// untested part
void sortStrings(char *arr[], int n);
void getMemoryStatus();
void getPsRamStatus(uint32_t psramSize);

/** scan for i2c devices and return first found one */
uint8_t findI2CDevice(const uint8_t startAddress = 20);

/**
 * align value to some bigger one, by blockSize
 * for example: roundBy(2737, 10) will return 2740
 */
uint32_t alignTo(uint32_t value, uint16_t blockSize);

#endif  // UTILS_H
