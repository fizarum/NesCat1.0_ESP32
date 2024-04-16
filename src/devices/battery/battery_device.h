#ifndef BATTERY_DEVICE_H
#define BATTERY_DEVICE_H

#include <base_device.h>
#include <inttypes.h>

// by voltage divider 2.2 mOhm & 4.7 mOhm
// and max battery voltage of 4.2V we can get 2.861V
#define BATTERY_LEVEL_MAX 2861

// by voltage divider 2.2 mOhm & 4.7 mOhm
// and min battery voltage of 2.8V we can get 1.907V
#define BATTERY_LEVEL_MIN 1907

class BatteryDevice : public BaseDevice {
 public:
  const char* const getName();
  bool onInit();
  void onUpdate();

  uint32_t getBatteryVoltage();

  /**
   * @brief Get the Battery Level in percents
   *
   * @return 0..100 %
   */
  uint8_t getBatteryLevel();

 private:
  uint32_t batteryLevel;
  int64_t lastUpdatedTimeAt;
  int64_t now;
};

#endif  // BATTERY_DEVICE_H