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
bool isInputChanged = false;
// callback for input event
void (*inputHandlerPtr)(uint16_t);

void isPressedStateForButtonSet(uint8_t bitPosInKeyState,
                                uint8_t bitPosOfButton);

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
void controlsInit(void (*onInputCallbackPtr)(uint16_t)) {
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

  inputHandlerPtr = onInputCallbackPtr;
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
bool isKeySet = false;

void requestKeysState() {
  if (now >= keyRequestedAt + delayBetweenKeyRequests) {
    keyRequestedAt = now;

    keyState = ~(pcf8575->read16());
    keyState &= GPIO_KEYS_MASK;

    if (lastReadKeyState == keyState) {
      return;
    }
    isInputChanged = false;

    // reset key state of all non joystick related buttons
    inputState &= KEYS_MASK;

    isPressedStateForButtonSet(0, TRIANGLE_BUTTON_BIT);
    isPressedStateForButtonSet(1, CIRCLE_BUTTON_BIT);
    isPressedStateForButtonSet(2, CROSS_BUTTON_BIT);
    isPressedStateForButtonSet(3, SQUARE_BUTTON_BIT);
    isPressedStateForButtonSet(4, SELECT_BUTTON_BIT);
    isPressedStateForButtonSet(5, START_BUTTON_BIT);

    if (isInputChanged == true) {
      debug("triggered keymap change: %u", inputState);
      inputHandlerPtr(inputState);
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
    isInputChanged = false;

    // left
    if (hPos <= JOY_NORMAL_VAL - JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 0);
      // reset flag of right button state
      inputState = bit::resetBit(inputState, 1);
      isInputChanged = true;
    }

    // right
    if (hPos >= JOY_NORMAL_VAL + JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 1);
      // reset flag of left button state
      inputState = bit::resetBit(inputState, 0);
      isInputChanged = true;
    }

    // down
    if (vPos <= JOY_NORMAL_VAL - JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 2);
      // reset flag of top button state
      inputState = bit::resetBit(inputState, 3);
      isInputChanged = true;
    }

    // up
    if (vPos >= JOY_NORMAL_VAL + JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 3);
      // reset flag of bottom button state
      inputState = bit::resetBit(inputState, 2);
      isInputChanged = true;
    }

    if (isInputChanged == true) {
      inputHandlerPtr(inputState);
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

/**
 * @brief update key press state according to keyState
 * @param bitPosInKeyState bit position (came from GPIO expander)
 * in keyState which is used to update key
 * @param bitPosOfButton position in output map (inputState) of key.
 * !important!
 * bitPosInKeyState != bitPosOfButton, check description on the top
 * of this file
 */
void isPressedStateForButtonSet(uint8_t bitPosInKeyState,
                                uint8_t bitPosOfButton) {
  isKeySet = bit::isBitSet(keyState, bitPosInKeyState);
  if (isKeySet == true) {
    isInputChanged = true;
  }
  inputState = bit::setBit16(inputState, bitPosOfButton, isKeySet);
}