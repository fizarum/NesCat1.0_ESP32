#include "log.h"

// comment to disable logs
#define DEBUG_ON

#ifdef DEBUG_ON
#include <HardwareSerial.h>
#endif

const uint8_t buffMaxLen = 30;
char debugBuff[buffMaxLen];

void debug(const char *message) {
#ifdef DEBUG_ON
  Serial.println(message);
#endif
}

void debug(const char *templateString, uint32_t value) {
  int len = strlen(templateString);
  if (len >= buffMaxLen) {
    sprintf(debugBuff, "value: %u", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
}

void debug(const char *templateString, uint8_t value) {
  int len = strlen(templateString);
  if (len >= buffMaxLen) {
    sprintf(debugBuff, "value: %d", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
}

void debug(const char *templateString, const char *value) {
  int len = strlen(templateString);
  const char *_value = value != nullptr ? value : "null";
  if (len >= buffMaxLen) {
    sprintf(debugBuff, "value: %s", _value);
  } else {
    sprintf(debugBuff, templateString, _value);
  }
  debug(debugBuff);
}

void debug(const char *templateString, const void *value) {
  int len = strlen(templateString);
  if (len >= buffMaxLen) {
    sprintf(debugBuff, "value: %p", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
}