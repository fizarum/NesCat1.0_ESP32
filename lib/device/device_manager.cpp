#include "device_manager.h"

#include <audio/audio_device.h>

void DeviceManager::init() {
  add(AUDIO_DEVICE, new AudioDevice());

  // config part - disable audio for now
  // get(AUDIO_DEVICE)->disable();
}