#ifndef JOYSTICK_DEVICE_H
#define JOYSTICK_DEVICE_H

#include <input_device/input_device.h>
#include <stdint.h>

#include "keymap.h"

class JoystickDevice : public InputDevice {
  bool onInit();
  void onUpdate();

 public:
  const char *const getName();
  void setCallback(void (*onInputCallbackPtr)(void));

  bool isLeftKeyDown();
  bool isLeftKeyUp();

  bool isRightKeyDown();
  bool isRightKeyUp();

  bool isDownKeyDown();
  bool isDownKeyUp();

  bool isUpKeyDown();
  bool isUpKeyUp();

  bool isSelectKeyDown();
  bool isSelectKeyUp();

  bool isStartKeyDown();
  bool isStartKeyUp();

  bool isMenuKeyDown();
  bool isMenuKeyUp();

  bool isAKeyDown();
  bool isAKeyUp();

  bool isBKeyDown();
  bool isBKeyUp();

  bool isXKeyDown();
  bool isXKeyUp();

  bool isYKeyDown();
  bool isYKeyUp();

  bool isLTKeyDown();
  bool isLTKeyUp();

  bool isRTKeyDown();
  bool isRTKeyUp();

  uint16_t keysState();
};

#endif  // JOYSTICK_DEVICE_H