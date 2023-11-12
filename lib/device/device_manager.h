#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <map>

#include "base_device.h"

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