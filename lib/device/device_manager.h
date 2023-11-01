#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <map>

#include "base_device.h"

const long DISPLAY_DEVICE = 100L;
const long AUDIO_DEVICE = 1000L;

class DeviceManager {
 private:
  std::map<long, BaseDevice *> devices = {};

 public:
  void init();

  void update() {
    for (const auto &kv : devices) {
      kv.second->update();
    }
  }

  void add(long id, BaseDevice *device) {
    devices[id] = device;
    if (device != nullptr) {
      device->init(id);
    }
  }

  BaseDevice *get(long id) { return devices[id]; }
};

#endif  // DEVICE_MANAGER_H