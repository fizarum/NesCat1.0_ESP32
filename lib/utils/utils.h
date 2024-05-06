#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Set (to 1) the bit in byte, source value isn't mutated
 *
 * @param source source byte
 * @param position of bit to change, counts from 0
 * @return uint8_t
 */
uint8_t setBit(uint8_t source, uint8_t position);

/**
 * @brief Reset (to 0) the bit in byte, source value isn't mutated
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
uint16_t setBit16(uint16_t source, uint8_t position);
uint32_t setBit32(uint32_t source, uint8_t position);

bool isBitSet(uint16_t source, uint8_t position);
bool isBitSet32(uint32_t source, uint8_t position);

/**
 * @brief modify current string by changing all
 * letters to lower case
 *
 * @param str string to transform
 */
void strToLowerCase(char *str);

/**
 * align value to some bigger one, by blockSize
 * for example: roundBy(2737, 10) will return 2740
 */
uint32_t alignTo(uint32_t value, uint16_t blockSize);

#ifdef __cplusplus
}
#endif

#endif  // UTILS_H
