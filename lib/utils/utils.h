#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

namespace bit {
/**
 * @brief Set (to 1) the bit in byte
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
uint8_t setBit(uint8_t source, uint8_t position);

/**
 * @brief Reset (to 0) the bit in byte
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
uint8_t resetBit(uint8_t source, uint8_t position);

/**
 * @brief Set or reset the bit in uint16_t, source value isn't mutated
 *
 * @param source source uint16_t
 * @param position of bit to change, counts from 0
 * @param isSet 0 means reset, > 0 - means set
 * @return updated copy of source value
 */
uint16_t setBit16(uint16_t source, uint8_t position, bool isSet = true);
uint32_t setBit32(uint32_t source, uint8_t position, bool isSet = true);
bool isBitSet(uint16_t source, uint8_t position);
bool isBitSet32(uint32_t source, uint8_t position);
}  // namespace bit

/**
 * @brief modify current string by changing all
 * letters to lower case
 *
 * @param str string to transform
 */
void strToLowerCase(char *str);

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
