#include "controls.h"

#include <PCF8574.h>
#include <esp32-hal-adc.h>
#include <utils.h>

#define ESP_INT_PIN 02
// count of connected lines to pcf8574 as buttons
#define CONNECTED_PINS 6

PCF8574 pcf8574(0x20);

uint8_t keyPressedFlag = 0;
uint8_t keysState;

// callback for keyboard
void (*onKeysCallbackPtr)(uint8_t);

// filters only pressed keys
uint8_t filterOnlyPressedKeys(uint8_t keysState, uint8_t keysStateLength);

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

unsigned long now;

// keyboard part
uint16_t delayBetweenKeyStateRequests = 50;
unsigned long lastKeyStateRequestedAt = 0;

// joystick part
uint16_t delayBetweenRequestsOfJoystic = 220;
unsigned long lastRequestedTimeOfJoystic = 0;

// request keys state: A,B,C,D
void requestKeysState();
void requestJoystickStateByPullMethod();

void _onKeyPressed() { keyPressedFlag = 1; }

/**
 * @brief should be called inside of setup() function
 */
void controlsInit(void (*onKeysCallback)(uint8_t)) {
  pinMode(ESP_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ESP_INT_PIN), _onKeyPressed, FALLING);

  pcf8574.pinMode(P0, INPUT);
  pcf8574.pinMode(P1, INPUT);
  pcf8574.pinMode(P2, INPUT);
  pcf8574.pinMode(P3, INPUT);
  pcf8574.pinMode(P4, INPUT);
  pcf8574.pinMode(P5, INPUT);

  if (pcf8574.begin()) {
    debug("init pcf8574... ok");
  } else {
    debug("init pcf8574... error");
  }
  onKeysCallbackPtr = onKeysCallback;
}

/**
 * @brief should be called inside of loop() function
 */
void controlsUpdate() {
  now = millis();
  requestKeysState();
  requestJoystickStateByPullMethod();
}

void requestKeysState() {
  if (now >= lastKeyStateRequestedAt + delayBetweenKeyStateRequests) {
    lastKeyStateRequestedAt = now;

    if (keyPressedFlag == 1) {
      keysState = pcf8574.digitalReadAll();

      // todo: remove on refactoring complete
      JOY_SQUARE = keysState & 0xf7;    // buttonD;
      JOY_CROSS = keysState & 0xfb;     // buttonC;
      JOY_CIRCLE = keysState & 0xfd;    // buttonB;
      JOY_TRIANGLE = keysState & 0xfe;  // buttonA;

      keysState = filterOnlyPressedKeys(keysState, CONNECTED_PINS);
      // todo: add more advanced filter mechanism to ignore released one of
      // several buttons
      if (keysState != 0) {
        onKeysCallbackPtr(keysState);
      }

      keyPressedFlag = 0;
    }
  }
}

// currently implemented analog joystick but later it should be replaced to
// "button" like one
void requestJoystickStateByPullMethod() {
  if (now >= lastRequestedTimeOfJoystic + delayBetweenRequestsOfJoystic) {
    // default range is from 0 to 4096
    uint16_t joystickHorizontalPos = analogRead(PIN_LEFT);
    uint16_t joystickVerticalPos = analogRead(PIN_UP);

    if (joystickHorizontalPos <= JOY_NORMAL_VAL - JOY_TRESHOLD) JOY_LEFT = 1;
    if (joystickHorizontalPos >= JOY_NORMAL_VAL + JOY_TRESHOLD) JOY_RIGHT = 1;
    if (joystickVerticalPos <= JOY_NORMAL_VAL - JOY_TRESHOLD) JOY_DOWN = 1;
    if (joystickVerticalPos >= JOY_NORMAL_VAL + JOY_TRESHOLD) JOY_UP = 1;

    lastRequestedTimeOfJoystic = now;
  }
}

/**
 * @brief filters only pressed keys
 *
 * @param keysState byte representing inverted value of pressed keys, for
 * example: 11111111 - no key pressed; 11111110 - first (by index) is pressed
 * @param keysStateLength the length of data which is processed. For example, if
 * only 5 buttons are connected this argument is 5 and only first 5 bits (from
 * keyState) is processed
 * @return uint8_t of reverted (and truncated by keysStateLength) keysState
 * value. For example if keysState: 11011110; keysStateLength: 5, result is:
 * 00000001
 * For same setup but keysStateLength: 6, result is: 00100001
 */
uint8_t filterOnlyPressedKeys(uint8_t keysState, uint8_t keysStateLength) {
  // 8 here is max uint8_t (byte) size in bits
  uint8_t partToIgnore = (8 - keysStateLength);

  uint8_t result = 0xff ^ keysState;
  // this part shifts left result on "partToIgnore" bytes to erase leading bytes
  result = result << partToIgnore;
  // this part shifts right result on "partToIgnore" bytes to fill leading bytes
  // by 0
  result = result >> partToIgnore;
  return result;
}