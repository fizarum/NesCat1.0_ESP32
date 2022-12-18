#include "controls.h"

#include <PCF8575.h>
#include <esp32-hal-adc.h>
#include <utils.h>

#define JOY_MAX_VAL 4096
// normal position of joystick - no keys pressed
#define JOY_NORMAL_VAL JOY_MAX_VAL / 2
// minimal step for joystick to register press event
#define JOY_THRESHOLD JOY_MAX_VAL / 10

PCF8575 *pcf8575 = nullptr;
uint8_t i2cAddress;
bool initialized = false;

/**
 * @brief all input joy/keys are represented as uint16_t value
 * keymap is next:
 * [0..3] - x/y information of joystick
 * [4   ] select button
 * [5   ] start button
 * [6   ] A (triangle) button
 * [7   ] B (circle) button
 * [8   ] C (cross) button
 * [9   ] D (square) button
 * [10  ] joystick middle
 *
 * !!!Important to remember!!!
 * map of IO ports (on GPIO expander) is next:
 * 0 - triangle
 * 1 - circle
 * 2 - cross
 * 3 - square
 * 4 - options (select)
 * 5 - start
 */
#define SELECT_BUTTON_BIT 4
#define START_BUTTON_BIT 5
#define TRIANGLE_BUTTON_BIT 6
#define CIRCLE_BUTTON_BIT 7
#define CROSS_BUTTON_BIT 8
#define SQUARE_BUTTON_BIT 9

#define PIN_JOY_UP 32
#define PIN_JOY_LEFT 33

// make sure that first 7 bits are only used
#define GPIO_KEYS_MASK 127

// bits from 4 to 10 (check description above) should be
// reset before apply new values from hardware
#define KEYS_MASK 63503

uint16_t inputState;
bool joystickStateUpdated;
// callback for keyboard
void (*onKeysCallbackPtr)(uint16_t);
void (*onJoystickMovedPtr)(uint16_t);

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
uint16_t delayBetweenKeyRequests = 50;
unsigned long keyRequestedAt = 0;

// joystick part
uint16_t delayBetweenRequestsOfJoystick = 220;
unsigned long lastRequestedTimeOfJoystick = 0;

// request keys state: A,B,C,D
void requestKeysState();
void requestJoystickStateByPullMethod();

/**
 * @brief should be called inside of setup() function
 */
void controlsInit(void (*onKeysCallback)(uint16_t),
                  void (*onJoystickMovedCallback)(uint16_t)) {
  Wire.begin();
  debug("scanning i2c bus...");
  i2cAddress = findI2CDevice();
  if (i2cAddress == 0) {
    debug("error during initializing keyboard!");
    initialized = false;
    return;
  }
  debug("found device on port: %x", i2cAddress);
  pcf8575 = new PCF8575(i2cAddress);
  pcf8575->begin(SDA, SCL);

  if (pcf8575->isConnected()) {
    debug("init pcf8575... ok");
  } else {
    debug("init pcf8575... error");
    initialized = false;
    return;
  }

  onKeysCallbackPtr = onKeysCallback;
  onJoystickMovedPtr = onJoystickMovedCallback;
  initialized = true;
}

/**
 * @brief should be called inside of loop() function
 */
void controlsUpdate() {
  if (initialized == false) {
    return;
  }
  now = millis();
  requestKeysState();
  requestJoystickStateByPullMethod();
}

uint16_t keyState = 0;
uint16_t lastReadKeyState = 0;

void requestKeysState() {
  if (now >= keyRequestedAt + delayBetweenKeyRequests) {
    keyRequestedAt = now;

    keyState = ~(pcf8575->read16());
    keyState &= GPIO_KEYS_MASK;

    if (lastReadKeyState == keyState) {
      return;
    }

    // reset key state of all non joystick related buttons
    inputState &= KEYS_MASK;

    JOY_TRIANGLE = bit::isBitSet(keyState, 0);
    inputState = bit::setBit16(inputState, TRIANGLE_BUTTON_BIT, JOY_TRIANGLE);

    JOY_CIRCLE = bit::isBitSet(keyState, 1);
    inputState = bit::setBit16(inputState, CIRCLE_BUTTON_BIT, JOY_CIRCLE);

    JOY_CROSS = bit::isBitSet(keyState, 2);
    inputState = bit::setBit16(inputState, CROSS_BUTTON_BIT, JOY_CROSS);

    JOY_SQUARE = bit::isBitSet(keyState, 3);
    inputState = bit::setBit16(inputState, SQUARE_BUTTON_BIT, JOY_SQUARE);

    JOY_OPTIONS = bit::isBitSet(keyState, 4);
    inputState = bit::setBit16(inputState, SELECT_BUTTON_BIT, JOY_OPTIONS);

    JOY_SHARE = bit::isBitSet(keyState, 5);
    inputState = bit::setBit16(inputState, START_BUTTON_BIT, JOY_SHARE);

    if (JOY_TRIANGLE == 1 || JOY_CIRCLE == 1 || JOY_CROSS == 1 ||
        JOY_SQUARE == 1 || JOY_OPTIONS == 1 || JOY_SHARE == 1) {
      debug("triggered keymap change: %u", inputState);
      onKeysCallbackPtr(inputState);
    }

    lastReadKeyState = keyState;
  }
}

uint16_t hPos, vPos;
// currently implemented analog joystick but later it should be replaced to
// "button" like one
void requestJoystickStateByPullMethod() {
  if (now >= lastRequestedTimeOfJoystick + delayBetweenRequestsOfJoystick) {
    // default range is from 0 to 4096
    hPos = analogRead(PIN_JOY_LEFT);
    vPos = analogRead(PIN_JOY_UP);
    joystickStateUpdated = false;

    // left
    if (hPos <= JOY_NORMAL_VAL - JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 0);
      // reset flag of right button state
      inputState = bit::resetBit(inputState, 1);
      JOY_LEFT = 1;
      joystickStateUpdated = true;
    }

    // right
    if (hPos >= JOY_NORMAL_VAL + JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 1);
      // reset flag of left button state
      inputState = bit::resetBit(inputState, 0);
      JOY_RIGHT = 1;
      joystickStateUpdated = true;
    }

    // down
    if (vPos <= JOY_NORMAL_VAL - JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 2);
      // reset flag of top button state
      inputState = bit::resetBit(inputState, 3);
      JOY_DOWN = 1;
      joystickStateUpdated = true;
    }

    // up
    if (vPos >= JOY_NORMAL_VAL + JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 3);
      // reset flag of bottom button state
      inputState = bit::resetBit(inputState, 2);
      JOY_UP = 1;
      joystickStateUpdated = true;
    }

    if (joystickStateUpdated == true) {
      onJoystickMovedPtr(inputState);
    }
    inputState = 0;

    lastRequestedTimeOfJoystick = now;
  }
}

bool isTrianglePressed(uint16_t state) {
  return bit::isBitSet(state, TRIANGLE_BUTTON_BIT);
}
bool isCirclePressed(uint16_t state) {
  return bit::isBitSet(state, CIRCLE_BUTTON_BIT);
}
bool isCrossPressed(uint16_t state) {
  return bit::isBitSet(state, CROSS_BUTTON_BIT);
}
bool isSquarePressed(uint16_t state) {
  return bit::isBitSet(state, SQUARE_BUTTON_BIT);
}

bool isSelectPressed(uint16_t state) {
  return bit::isBitSet(state, SELECT_BUTTON_BIT);
}
bool isStartPressed(uint16_t state) {
  return bit::isBitSet(state, START_BUTTON_BIT);
}

bool isLeftPressed(uint16_t state) { return bit::isBitSet(state, 0); }
bool isRightPressed(uint16_t state) { return bit::isBitSet(state, 1); }
bool isDownPressed(uint16_t state) { return bit::isBitSet(state, 2); }
bool isUpPressed(uint16_t state) { return bit::isBitSet(state, 3); }
