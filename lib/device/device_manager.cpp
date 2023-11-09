#include "device_manager.h"

#include <log.h>

#include "audio/audio_device.h"
#include "controls/joystick_device.h"
#include "display/display_device.h"
#include "storage/storage_device.h"

std::map<uint8_t, BaseDevice *> *_devices = nullptr;

const char *const displayName = "display";
const char *const audioName = "audio";
const char *const joystickName = "joystick";
const char *const storageName = "storage";

const char *getDeviceName(uint8_t deviceId);

void DeviceManager::init() {
  add(DISPLAY_DEVICE, new DisplayDevice());
  add(AUDIO_DEVICE, new AudioDevice());
  add(JOYSTICK_DEVICE, new JoystickDevice());
  add(STORAGE_DEVICE, new StorageDevice());

  // config part - disable audio for now
  // get(AUDIO_DEVICE)->disable();

  _devices = &(this->devices);
}

void DeviceManager::add(uint8_t id, BaseDevice *device) {
  if (device == nullptr) return;
  this->devices[id] = device;
  const char *deviceName = getDeviceName(id);
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

BaseDevice *DeviceManager::get(uint8_t id) { return _devices->at(id); }

const char *getDeviceName(uint8_t deviceId) {
  switch (deviceId) {
    case DISPLAY_DEVICE:
      return displayName;

    case AUDIO_DEVICE:
      return audioName;

    case JOYSTICK_DEVICE:
      return joystickName;

    case STORAGE_DEVICE:
      return storageName;

    default:
      return nullptr;
  }
}