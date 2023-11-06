#include "device_manager.h"

#include "audio/audio_device.h"
#include "display/display.h"

void DeviceManager::init() {
  add(DISPLAY_DEVICE, new DisplayDevice());
  add(AUDIO_DEVICE, new AudioDevice());

  // config part - disable audio for now
  // get(AUDIO_DEVICE)->disable();
}