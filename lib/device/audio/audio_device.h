#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

#include "../base_device.h"

class AudioDevice : public BaseDevice {
 public:
  bool onInit();
  void onUpdate();
  void onEnabled(bool enabled);
};

#endif  // AUDIO_DEVICE_H