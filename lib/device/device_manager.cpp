#include "device_manager.h"

#include <log.h>

std::map<uint8_t, BaseDevice *> *_devices = nullptr;

void DeviceManager::init() { _devices = &(this->devices); }

void DeviceManager::add(uint8_t id, BaseDevice *device) {
  if (device == nullptr) return;
  this->devices[id] = device;
  const char *deviceName = device->getName();
  if (deviceName == nullptr) {
    debug("can't find such device!");
    return;
  }
  if (device->init(id, deviceName) == true) {
    debug("init %s: ok", deviceName);
  } else {
    debug("init %s: err!", deviceName);
  }
}

BaseDevice *DeviceManager::get(uint8_t id) {
  try {
    return _devices->at(id);
  } catch (std::out_of_range e) {
    return nullptr;
  }
}