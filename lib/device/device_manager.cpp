#include "device_manager.h"

#include "audio/audio_device.h"
#include "controls/joystick_device.h"
#include "display/display_device.h"

void DeviceManager::init() {
  add(DISPLAY_DEVICE, new DisplayDevice());
  add(AUDIO_DEVICE, new AudioDevice());
  add(JOYSTICK_DEVICE, new JoystickDevice());

  // config part - disable audio for now
  // get(AUDIO_DEVICE)->disable();
}