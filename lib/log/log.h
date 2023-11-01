#ifndef LOG_H
#define LOG_H

#include <stdint.h>

void debug(const char *message);
void debug(const char *templateString, uint32_t value);
void debug(const char *templateString, const char *value);
void debug(const char *templateString, const void *value);

#endif  // LOG_H