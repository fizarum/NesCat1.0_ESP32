#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define MAXFILENAME_LENGTH 64

void debug(const char *message);
void debug(const char *templateString, uint32_t value);
void debug(const char *templateString, const char *value);

// untested part
void sortStrings(char *arr[], int n);
void getMemoryStatus();
void getPsRamStatus(uint32_t psramSize);

#endif  // UTILS_H
