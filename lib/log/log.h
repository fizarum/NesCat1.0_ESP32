#ifndef LOG_H
#define LOG_H

#include <stdint.h>
/**
 * @brief Standard minimalistic version of printing messages into
 * serial port
 *
 * @param message to print
 */
void debug(const char *message);

/**
 * @brief print formatted message with uint32_t number into serial port
 *
 * @param templateString which contains text and '%u' for number printing
 * @param value or number to be printed in message
 */
void debug(const char *templateString, uint32_t value);

/**
 * @brief print formatted message with const char[] string into serial port
 *
 * @param templateString which contains text and '%s' for string printing
 * @param value string to be printed in message
 */
void debug(const char *templateString, const char *value);

/**
 * @brief print formatted message with pointer data into serial port
 *
 * @param templateString which contains text and '%p' for address printing
 * @param value or pointer, address which to be printed in message
 */
void debug(const char *templateString, const void *value);

#endif  // LOG_H