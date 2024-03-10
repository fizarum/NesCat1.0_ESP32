#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include <stdint.h>

#include "../base_device.h"

class InputDevice : public BaseDevice {
 public:
  virtual bool isLeftPressed() = 0;
  virtual bool isRightPressed() = 0;
  virtual bool isDownPressed() = 0;
  virtual bool isUpPressed() = 0;

  virtual bool isSelectPressed() = 0;
  virtual bool isStartPressed() = 0;
  virtual bool isMenuPressed() = 0;

  virtual bool isAPressed() = 0;
  virtual bool isBPressed() = 0;
  virtual bool isXPressed() = 0;
  virtual bool isYPressed() = 0;

  virtual bool isLeftTriggerPressed() = 0;
  virtual bool isRightTriggerPressed() = 0;

  virtual uint16_t keysState() = 0;
};

#endif  // INPUT_DEVICE_H