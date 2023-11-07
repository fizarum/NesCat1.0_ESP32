#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

#include "base_device.h"
#include "keymap.h"

class JoystickDevice : public BaseDevice {
  void onInit();
  void onUpdate();

 public:
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

#endif  // CONTROLS_H