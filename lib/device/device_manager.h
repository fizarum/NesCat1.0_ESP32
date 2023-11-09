#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <map>

#include "base_device.h"
const uint8_t DISPLAY_DEVICE = 100;
const uint8_t AUDIO_DEVICE = 101;
const uint8_t JOYSTICK_DEVICE = 102;
const uint8_t STORAGE_DEVICE = 103;

class DeviceManager {
 private:
  std::map<uint8_t, BaseDevice *> devices = {};

 public:
  void init();

  void update() {
    for (const auto &kv : devices) {
      BaseDevice *device = kv.second;
      if (device->enabled()) {
        device->update();
      }
    }
  }

  void add(uint8_t id, BaseDevice *device);

  static BaseDevice *get(uint8_t id);
};

#endif  // DEVICE_MANAGER_H