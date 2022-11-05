#include "controls.h"

#include <PCF8575.h>
#include <esp32-hal-adc.h>
#include <utils.h>

#define JOY_MAX_VAL 4096
// normal postition of joystic - no keys pressed
#define JOY_NORMAL_VAL JOY_MAX_VAL / 2
// minimal step for joystic to register press event
#define JOY_THRESHOLD JOY_MAX_VAL / 10

#define I2C_ADDRESS 0x25

#define ESP_SDA_PIN 21
#define ESP_SCL_PIN 22
#define ESP_INT_PIN 02

PCF8575 pcf8575(I2C_ADDRESS);

uint8_t keyPressedFlag = 0;

/**
 * @brief all input joy/keys are represented as uint16_t value
 * keymap is next:
 * [0..3] - x/y information of joystick
 * [4   ] start button
 * [5   ] select button
 * [6   ] A (triangle) button
 * [7   ] B (circle) button
 * [8   ] C (cross) button
 * [9   ] D (square) button
 * [10  ] joystick middle
 */
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

void onKeyPressedInt() { keyPressedFlag = 1; }

/**
 * @brief should be called inside of setup() function
 */
void controlsInit(void (*onKeysCallback)(uint16_t),
                  void (*onJoystickMovedCallback)(uint16_t)) {
  pinMode(ESP_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ESP_INT_PIN), onKeyPressedInt, FALLING);

  pcf8575.begin(ESP_SDA_PIN, ESP_SCL_PIN);

  if (pcf8575.isConnected()) {
    debug("init pcf8575... ok");
  } else {
    debug("init pcf8575... error");
  }

  onKeysCallbackPtr = onKeysCallback;
  onJoystickMovedPtr = onJoystickMovedCallback;
}

/**
 * @brief should be called inside of loop() function
 */
void controlsUpdate() {
  now = millis();
  requestKeysState();
  requestJoystickStateByPullMethod();
}

uint16_t tempKeyState;

void requestKeysState() {
  if (now >= keyRequestedAt + delayBetweenKeyRequests) {
    keyRequestedAt = now;

    if (keyPressedFlag == 1) {
      tempKeyState = ~pcf8575.read16();

      JOY_TRIANGLE = bit::isBitSet(tempKeyState, 0);
      inputState = bit::setBit16(tempKeyState, 6, JOY_TRIANGLE);

      JOY_CIRCLE = bit::isBitSet(tempKeyState, 1);
      inputState = bit::setBit16(tempKeyState, 7, JOY_CIRCLE);

      JOY_CROSS = bit::isBitSet(tempKeyState, 2);
      inputState = bit::setBit16(tempKeyState, 8, JOY_CROSS);

      JOY_SQUARE = bit::isBitSet(tempKeyState, 3);
      inputState = bit::setBit16(tempKeyState, 9, JOY_SQUARE);

      onKeysCallbackPtr(inputState);

      keyPressedFlag = 0;
    }
  }
}

uint16_t hPos, vPos;
// currently implemented analog joystick but later it should be replaced to
// "button" like one
void requestJoystickStateByPullMethod() {
  if (now >= lastRequestedTimeOfJoystick + delayBetweenRequestsOfJoystick) {
    // default range is from 0 to 4096
    hPos = analogRead(PIN_LEFT);
    vPos = analogRead(PIN_UP);
    joystickStateUpdated = false;

    // left
    if (hPos <= JOY_NORMAL_VAL - JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 0);
      JOY_LEFT = 1;
      joystickStateUpdated = true;
    }

    // right
    if (hPos >= JOY_NORMAL_VAL + JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 1);
      JOY_RIGHT = 1;
      joystickStateUpdated = true;
    }

    // down
    if (vPos <= JOY_NORMAL_VAL - JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 2);
      JOY_DOWN = 1;
      joystickStateUpdated = true;
    }

    // top
    if (vPos >= JOY_NORMAL_VAL + JOY_THRESHOLD) {
      inputState = bit::setBit16(inputState, 3);
      JOY_UP = 1;
      joystickStateUpdated = true;
    }

    if (joystickStateUpdated == true) {
      onJoystickMovedPtr(inputState);
    }

    lastRequestedTimeOfJoystick = now;
  }
}