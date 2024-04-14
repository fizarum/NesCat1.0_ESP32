#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include <stdint.h>

#include "../base_device.h"

class InputDevice : public BaseDevice {
 public:
  /**
   * @brief indicates if left button pressed but not released(longpress)
   */
  virtual bool isLeftKeyDown() = 0;

  /**
   * @brief indicates if left button was released
   */
  virtual bool isLeftKeyUp() = 0;

  /**
   * @brief indicates if right button pressed but not released(longpress)
   */
  virtual bool isRightKeyDown() = 0;

  /**
   * @brief indicates if right button was released
   */
  virtual bool isRightKeyUp() = 0;

  /**
   * @brief indicates if down button pressed but not released(longpress)
   */
  virtual bool isDownKeyDown() = 0;

  /**
   * @brief indicates if down button was released
   */
  virtual bool isDownKeyUp() = 0;

  /**
   * @brief indicates if up button pressed but not released(longpress)
   */
  virtual bool isUpKeyDown() = 0;

  /**
   * @brief indicates if up button was released
   */
  virtual bool isUpKeyUp() = 0;

  /**
   * @brief indicates if "select" button pressed but not released(longpress)
   */
  virtual bool isSelectKeyDown() = 0;

  /**
   * @brief indicates if "select" button was released
   */
  virtual bool isSelectKeyUp() = 0;

  /**
   * @brief indicates if "start" button pressed but not released(longpress)
   */
  virtual bool isStartKeyDown() = 0;

  /**
   * @brief indicates if "start" button was released
   */
  virtual bool isStartKeyUp() = 0;

  /**
   * @brief indicates if "menu" button pressed but not released(longpress)
   */
  virtual bool isMenuKeyDown() = 0;

  /**
   * @brief indicates if "menu" button was released
   */
  virtual bool isMenuKeyUp() = 0;

  /**
   * @brief indicates if "A" button pressed but not released(longpress)
   */
  virtual bool isAKeyDown() = 0;

  /**
   * @brief indicates if "A" button was released
   */
  virtual bool isAKeyUp() = 0;

  /**
   * @brief indicates if "B" button pressed but not released(longpress)
   */
  virtual bool isBKeyDown() = 0;

  /**
   * @brief indicates if "B" button was released
   */
  virtual bool isBKeyUp() = 0;

  /**
   * @brief indicates if "X" button pressed but not released(longpress)
   */
  virtual bool isXKeyDown() = 0;

  /**
   * @brief indicates if "X" button was released
   */
  virtual bool isXKeyUp() = 0;

  /**
   * @brief indicates if "Y" button pressed but not released(longpress)
   */
  virtual bool isYKeyDown() = 0;

  /**
   * @brief indicates if "Y" button was released
   */
  virtual bool isYKeyUp() = 0;

  /**
   * @brief indicates if Left Trigger button pressed but not released(longpress)
   */
  virtual bool isLTKeyDown() = 0;

  /**
   * @brief indicates if Left Trigger button was released
   */
  virtual bool isLTKeyUp() = 0;

  /**
   * @brief indicates if Right Trigger button pressed but not
   * released(longpress)
   */
  virtual bool isRTKeyDown() = 0;

  /**
   * @brief indicates if Right Trigger button was released
   */
  virtual bool isRTKeyUp() = 0;

  virtual uint16_t keysState() = 0;
};

#endif  // INPUT_DEVICE_H