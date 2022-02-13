#include "controls.h"

#include <PCF8574.h>
#include <esp32-hal-adc.h>

#define ESP_INT_PIN 02

PCF8574 pcf8574(0x20);

uint8_t keyPressedFlag = 0;
PCF8574::DigitalInput keysState;

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

// button with triangle symbol
uint8_t buttonA = 0;
// button with circle symbol
uint8_t buttonB = 0;
// button with cross symbol
uint8_t buttonC = 0;
// button with square symbol
uint8_t buttonD = 0;

// pull method section
uint16_t delayBetweenRequests = 300;
unsigned long lastRequestedTime = 0;
uint32_t requestTime;
// end section

void updateJoystickByPullMethod();

void _onKeyPressed() { keyPressedFlag = 1; }

/**
 * @brief should be called inside of setup() function
 */
void controlsInit() {
  pinMode(ESP_INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ESP_INT_PIN), _onKeyPressed, FALLING);

  pcf8574.pinMode(P0, INPUT);
  pcf8574.pinMode(P1, INPUT);
  pcf8574.pinMode(P2, INPUT);
  pcf8574.pinMode(P3, INPUT);
  pcf8574.pinMode(P4, INPUT);
  pcf8574.pinMode(P5, INPUT);

  if (pcf8574.begin()) {
    Serial.println("init pcf8574... ok");
  } else {
    Serial.println("init pcf8574... error");
  }
}

/**
 * @brief should be called inside of loop() function
 */
void controlsUpdate() {
  if (keyPressedFlag == 1) {
    keysState = pcf8574.digitalReadAll();

    buttonA = keysState.p0;
    buttonB = keysState.p1;
    buttonC = keysState.p2;
    buttonD = keysState.p3;

    // todo: remove on refactoring complete
    JOY_CROSS = buttonC;
    JOY_SQUARE = buttonD;
    JOY_CIRCLE = buttonB;
    JOY_TRIANGLE = buttonA;

    keyPressedFlag = 0;
  }
  updateJoystickByPullMethod();
}

// currently implemented analog joystick but later it should be replaced to
// "button" like one
void updateJoystickByPullMethod() {
  requestTime = millis();
  if (requestTime >= lastRequestedTime + delayBetweenRequests) {
    // default range is from 0 to 4096
    uint16_t joystickHorizontalPos = analogRead(PIN_LEFT);
    uint16_t joystickVerticalPos = analogRead(PIN_UP);

    if (joystickHorizontalPos <= JOY_NORMAL_VAL - JOY_TRESHOLD) JOY_LEFT = 1;
    if (joystickHorizontalPos >= JOY_NORMAL_VAL + JOY_TRESHOLD) JOY_RIGHT = 1;
    if (joystickVerticalPos <= JOY_NORMAL_VAL - JOY_TRESHOLD) JOY_DOWN = 1;
    if (joystickVerticalPos >= JOY_NORMAL_VAL + JOY_TRESHOLD) JOY_UP = 1;

    lastRequestedTime = requestTime;
  }
}