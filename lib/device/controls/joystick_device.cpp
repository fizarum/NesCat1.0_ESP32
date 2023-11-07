#include "joystick_device.h"

#include <MCP23017.h>
#include <log.h>
#include <utils.h>

#include "keymap.h"

#define JOY_MAX_VAL 4096
// normal position of joystick - no keys pressed
#define JOY_NORMAL_VAL JOY_MAX_VAL / 2
// minimal step for joystick to register press event
#define JOY_THRESHOLD JOY_MAX_VAL / 10

MCP23017 *mcp = nullptr;
uint8_t i2cAddress;
bool initialized = false;

// make sure that last 3 bits not used (they are disabled)
#define GPIO_KEYS_MASK 0xFFF8

/**
 * @brief keymap used for storing input data:
 * [0  ] - left arrow bit of joystick
 * [1  ] - right arrow bit of joystick
 * [2  ] - up arrow bit of joystick
 * [3  ] - down arrow bit of joystick
 *
 * menu keys:
 * [4  ] select button
 * [5  ] start button
 * [6  ] menu button
 *
 * [7  ] A (triangle) button
 * [8  ] B (circle) button
 * [9  ] X (cross) button
 * [10 ] Y (square) button
 *
 * [11 ] left trigger
 * [12 ] right trigger
 */
uint16_t keymap = 0;

/** cached flag if input changed*/
bool isInputChanged = false;

bool bindKeymap(uint16_t gpioMap);

// callback for input event
void (*inputHandlerPtr)(void);

unsigned long now;

// keyboard part
uint16_t delayBetweenKeyRequests = 50;
unsigned long keyRequestedAt = 0;

// joystick part
uint16_t delayBetweenRequestsOfJoystick = 220;
unsigned long lastRequestedTimeOfJoystick = 0;

// request keys state: A,B,C,D
void requestKeysState();

void JoystickDevice::onInit() {
  Wire.begin();
  i2cAddress = findI2CDevice();
  if (i2cAddress == 0) {
    debug("error during initializing keyboard!");
    initialized = false;
    debug("init keyboard... error");
    return;
  }
  debug("found keyboard on port: %x", i2cAddress);
  mcp = new MCP23017(i2cAddress, Wire);
  mcp->init();

  // Port A & B as output
  mcp->portMode(MCP23017Port::A, 0xFF);
  mcp->portMode(MCP23017Port::B, 0xFF);

  // Reset port A & B values
  mcp->writeRegister(MCP23017Register::GPIO_A, 0x00);
  mcp->writeRegister(MCP23017Register::GPIO_B, 0x00);

  // GPIO_A $ GPIO_B reflects the opposite logic of all pins state
  mcp->writeRegister(MCP23017Register::IPOL_A, 0xFF);
  mcp->writeRegister(MCP23017Register::IPOL_B, 0xFF);
  debug("init keyboard... ok");

  initialized = true;
}

void JoystickDevice::onUpdate() {
  if (initialized == false) {
    return;
  }
  now = millis();
  requestKeysState();
}

void JoystickDevice::setCallback(void (*onInputCallbackPtr)(void)) {
  inputHandlerPtr = onInputCallbackPtr;
}

uint16_t lastReadKeyState = 0;
bool isKeySet = false;
uint16_t gpioKeymap = 0;

void requestKeysState() {
  if (now >= keyRequestedAt + delayBetweenKeyRequests) {
    keyRequestedAt = now;
    gpioKeymap = mcp->read();

    gpioKeymap &= GPIO_KEYS_MASK;
    if (lastReadKeyState == gpioKeymap) {
      return;
    }

    bindKeymap(gpioKeymap);

    if (isInputChanged == true) {
      debug("triggered keymap change: %u", keymap);
      inputHandlerPtr();
    }

    lastReadKeyState = keymap;
  }
}

bool JoystickDevice::isLeftPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_LEFT);
}
bool JoystickDevice::isRightPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_RIGHT);
}
bool JoystickDevice::isDownPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_DOWN);
}
bool JoystickDevice::isUpPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_UP);
}

bool JoystickDevice::isSelectPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_SELECT);
}
bool JoystickDevice::isStartPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_START);
}
bool JoystickDevice::isMenuPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_MENU);
}

bool JoystickDevice::isAPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_A);
}
bool JoystickDevice::isBPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_B);
}
bool JoystickDevice::isXPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_X);
}
bool JoystickDevice::isYPressed() {
  return bit::isBitSet(keymap, GPIO_BUTTON_Y);
}

bool JoystickDevice::isLeftTriggerPressed() {
  return bit::isBitSet(keymap, GPIO_TRIGGER_LEFT);
}
bool JoystickDevice::isRightTriggerPressed() {
  return bit::isBitSet(keymap, GPIO_TRIGGER_RIGHT);
}

uint16_t JoystickDevice::keysState() { return keymap; }

/**
 * @brief update key press state according to gpio map
 * @param gpioMap bit map from gpio expander
 * @returns if keymap has been updated
 */
bool bindKeymap(uint16_t gpioMap) {
  isInputChanged = gpioMap != keymap;
  keymap = gpioMap;

  return isInputChanged;
}