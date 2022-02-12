#include "controls.h"

#include <esp32-hal-adc.h>

// todo: rework on one uint8_t variable for joystick (at least)
//  INPUT SYSTEM:
uint8_t JOY_UP = 0;
uint8_t JOY_DOWN = 0;
uint8_t JOY_LEFT = 0;
uint8_t JOY_RIGHT = 0;
uint8_t JOY_CROSS = 0;
uint8_t JOY_SQUARE = 0;
uint8_t JOY_CIRCLE = 0;
uint8_t JOY_TRIANGLE = 0;
uint8_t JOY_SHARE = 0;    //(START)
uint8_t JOY_OPTIONS = 0;  //(SELECT)

// currently implemented analog joystick but later it should be replaced to
// "button" like one
uint8_t isJoystickMoved() {
  // default range is from 0 to 4096
  uint16_t joystickHorizontalPos = analogRead(PIN_LEFT);
  uint16_t joystickVerticalPos = analogRead(PIN_UP);
  // debug("joy horizontal: %u", joysticHorizontalPos);
  // debug("joy vertical: %u", joysticVerticalPos);

  if (joystickHorizontalPos <= JOY_NORMAL_VAL - JOY_TRESHOLD) {
    JOY_LEFT = 1;
    return 1;
  }

  if (joystickHorizontalPos >= JOY_NORMAL_VAL + JOY_TRESHOLD) {
    JOY_RIGHT = 1;
    return 1;
  }

  if (joystickVerticalPos <= JOY_NORMAL_VAL - JOY_TRESHOLD) {
    JOY_DOWN = 1;
    return 1;
  }

  if (joystickVerticalPos >= JOY_NORMAL_VAL + JOY_TRESHOLD) {
    JOY_UP = 1;
    return 1;
  }

  if (digitalRead(PIN_START) == 1) {
    JOY_SHARE = 1;  // START
    return 1;
  }
  if (digitalRead(PIN_A) == 1) {
    JOY_SQUARE = 1;  // A
    return 1;
  }
  return 0;
}