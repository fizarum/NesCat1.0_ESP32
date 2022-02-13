#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

#define MAXFILENAME_LENGTH 64

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

// untested part

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

#endif  // UTILS_H
