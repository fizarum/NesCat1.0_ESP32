#include "log.h"

#include "../configurator/configurator.h"

#ifdef DEBUG_ON
#ifndef PORT_SDK
#include <HardwareSerial.h>
#else
#include <stdio.h>
#include <string.h>
#endif  // PORT_SDK
#endif  // DEBUG_ON

#ifdef DEBUG_ON
#define DEBUG_STRING_MAX_LEN 64
char debugBuff[DEBUG_STRING_MAX_LEN];
#endif  // DEBUG_ON

void debug(const char *message) {
#ifdef DEBUG_ON
#ifndef PORT_SDK
  Serial.println(message);
#else
  printf(message);
#endif  // PORT_SDK
#endif  // DEBUG_ON
}

void debug(const char *templateString, uint32_t value) {
#ifdef DEBUG_ON
  int len = strlen(templateString);
  if (len >= DEBUG_STRING_MAX_LEN) {
    sprintf(debugBuff, "value: %u", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
#endif  // DEBUG_ON
}

void debug(const char *templateString, const char *value) {
#ifdef DEBUG_ON
  int len = strlen(templateString);
  const char *_value = value != nullptr ? value : "null";
  if (len >= DEBUG_STRING_MAX_LEN) {
    sprintf(debugBuff, "value: %s", _value);
  } else {
    sprintf(debugBuff, templateString, _value);
  }
  debug(debugBuff);
#endif  // DEBUG_ON
}

void debug(const char *templateString, const void *value) {
#ifdef DEBUG_ON
  int len = strlen(templateString);
  if (len >= DEBUG_STRING_MAX_LEN) {
    sprintf(debugBuff, "value: %p", value);
  } else {
    sprintf(debugBuff, templateString, value);
  }
  debug(debugBuff);
#endif  // DEBUG_ON
}