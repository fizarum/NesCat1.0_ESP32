#include "joystick_device.h"

#include <MCP23017.h>
#include <log.h>
#include <utils.h>

#include "keymap.h"

MCP23017 *mcp = nullptr;

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
uint16_t previousChangedKeymap = 0;

// callback for input event
void (*inputHandlerPtr)(void);

unsigned long now;

// keyboard part
uint16_t delayBetweenKeyRequests = 50;
unsigned long keyRequestedAt = 0;

// joystick part
uint16_t delayBetweenRequestsOfJoystick = 220;
unsigned long lastRequestedTimeOfJoystick = 0;

uint8_t findI2CDevice(const uint8_t startAddress);
const char *const JoystickDevice::getName() { return "joystick"; }

bool JoystickDevice::onInit() {
  Wire.begin();
  uint8_t i2cAddress = findI2CDevice(20);
  if (i2cAddress == 0) {
    return false;
  }
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

  return true;
}
bool isKeymapChanged = false;

void JoystickDevice::onUpdate() {
  now = millis();

  if (now < keyRequestedAt + delayBetweenKeyRequests) return;

  keyRequestedAt = now;
  keymap = mcp->read();
  keymap &= GPIO_KEYS_MASK;

  if (previousChangedKeymap == 0 || previousChangedKeymap != keymap) {
    isKeymapChanged = true;
    inputHandlerPtr();
    previousChangedKeymap = keymap;
    isKeymapChanged = false;
  } else {
    inputHandlerPtr();
  }
}

void JoystickDevice::setCallback(void (*onInputCallbackPtr)(void)) {
  inputHandlerPtr = onInputCallbackPtr;
}

/**
 * @brief helper function to check if key is released after pressed
 * @param gpioButton any key declared in keymap.h (like: GPIO_BUTTON_LEFT)
 */
static const inline bool isKeyReleased(uint8_t gpioButton) {
  return isBitSet(previousChangedKeymap, gpioButton) &&
         isBitSet(keymap, gpioButton) == false;
}

bool JoystickDevice::isLeftKeyDown() {
  return isBitSet(keymap, GPIO_BUTTON_LEFT);
}

bool JoystickDevice::isLeftKeyUp() { return isKeyReleased(GPIO_BUTTON_LEFT); }

bool JoystickDevice::isRightKeyDown() {
  return isBitSet(keymap, GPIO_BUTTON_RIGHT);
}

bool JoystickDevice::isRightKeyUp() { return isKeyReleased(GPIO_BUTTON_RIGHT); }

bool JoystickDevice::isDownKeyDown() {
  return isBitSet(keymap, GPIO_BUTTON_DOWN);
}

bool JoystickDevice::isDownKeyUp() { return isKeyReleased(GPIO_BUTTON_DOWN); }

bool JoystickDevice::isUpKeyDown() { return isBitSet(keymap, GPIO_BUTTON_UP); }

bool JoystickDevice::isUpKeyUp() { return isKeyReleased(GPIO_BUTTON_UP); }

bool JoystickDevice::isSelectKeyDown() {
  return isBitSet(keymap, GPIO_BUTTON_SELECT);
}

bool JoystickDevice::isSelectKeyUp() {
  return isKeyReleased(GPIO_BUTTON_SELECT);
}

bool JoystickDevice::isStartKeyDown() {
  return isBitSet(keymap, GPIO_BUTTON_START);
}

bool JoystickDevice::isStartKeyUp() { return isKeyReleased(GPIO_BUTTON_START); }

bool JoystickDevice::isMenuKeyDown() {
  return isBitSet(keymap, GPIO_BUTTON_MENU);
}

bool JoystickDevice::isMenuKeyUp() { return isKeyReleased(GPIO_BUTTON_MENU); }

bool JoystickDevice::isAKeyDown() { return isBitSet(keymap, GPIO_BUTTON_A); }

bool JoystickDevice::isAKeyUp() { return isKeyReleased(GPIO_BUTTON_A); }

bool JoystickDevice::isBKeyDown() { return isBitSet(keymap, GPIO_BUTTON_B); }

bool JoystickDevice::isBKeyUp() { return isKeyReleased(GPIO_BUTTON_B); }

bool JoystickDevice::isXKeyDown() { return isBitSet(keymap, GPIO_BUTTON_X); }

bool JoystickDevice::isXKeyUp() { return isKeyReleased(GPIO_BUTTON_X); }

bool JoystickDevice::isYKeyDown() { return isBitSet(keymap, GPIO_BUTTON_Y); }

bool JoystickDevice::isYKeyUp() { return isKeyReleased(GPIO_BUTTON_Y); }

bool JoystickDevice::isLTKeyDown() {
  return isBitSet(keymap, GPIO_LEFT_TRIGGER);
}

bool JoystickDevice::isLTKeyUp() { return isKeyReleased(GPIO_LEFT_TRIGGER); }

bool JoystickDevice::isRTKeyDown() {
  return isBitSet(keymap, GPIO_RIGHT_TRIGGER);
}

bool JoystickDevice::isRTKeyUp() { return isKeyReleased(GPIO_RIGHT_TRIGGER); }

uint16_t JoystickDevice::keysState() { return keymap; }

uint8_t findI2CDevice(const uint8_t startAddress) {
  uint8_t address, response;
  for (address = startAddress; address < 127; address++) {
    Wire.beginTransmission(address);
    response = Wire.endTransmission();

    if (response == 0) {
      debug("found device on port: %x", address);
      return address;
    }
  }
  debug("device not found in i2c port");
  return 0;
}