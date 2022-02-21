#include "utils.h"

#include <Arduino.h>

// comment to disable logs
#define DEBUG

const uint8_t buffMaxLen = 30;

char debugBuff[buffMaxLen];

void debug(const char *message) {
#ifdef DEBUG
  Serial.println(message);
#endif
}

void debug(const char *templateString, uint32_t value) {
  int strlength = strlen(templateString);
  if (strlength >= buffMaxLen) {
    sprintf(debugBuff, "value: %u", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
}

void debug(const char *templateString, const char *value) {
  int strlength = strlen(templateString);
  if (strlength >= buffMaxLen) {
    sprintf(debugBuff, "value: %s", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
}

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

void sortStrings(char *arr[], int n) {
  char temp[MAXFILENAME_LENGTH];

  // Sorting strings using bubble sort
  for (int j = 0; j < n - 1; j++) {
    for (int i = j + 1; i < n; i++) {
      if (strcmp(arr[j], arr[i]) > 0) {
        strcpy(temp, arr[j]);
        strcpy(arr[j], arr[i]);
        strcpy(arr[i], temp);
      }
    }
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