#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
const uint8_t buffMaxLen = 30;

char debugBuff[buffMaxLen];

void debug(const char *message) {
  #ifdef DEBUG
    Serial.println(message);
  #endif
}

void debug(const char *templateString, int strlength, int value) {
  if(strlength >= buffMaxLen) {
    sprintf(debugBuff, "value: %d", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
}

#endif //UTILS_H
