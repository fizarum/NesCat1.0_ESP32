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
  bool isLeftPressed();
  bool isRightPressed();
  bool isDownPressed();
  bool isUpPressed();

  bool isSelectPressed();
  bool isStartPressed();
  bool isMenuPressed();

  bool isAPressed();
  bool isBPressed();
  bool isXPressed();
  bool isYPressed();

  bool isLeftTriggerPressed();
  bool isRightTriggerPressed();

  uint16_t keysState();
};

#endif  // JOYSTICK_DEVICE_H