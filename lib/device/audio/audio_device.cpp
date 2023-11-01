#include "audio_device.h"

#include <Audio.h>
#include <log.h>

#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26

#define SOUND_ENABLED true
#define DEFAULT_SAMPLE_RATE 24000

Audio *audio = nullptr;

void AudioDevice::onInit() {
  audio = new Audio();
  bool isSet = audio->setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  if (isSet == true) {
    debug("init sound... ok");
    audio->setVolume(0);
    audio->forceMono(true);
    debug("volume set to: %d\n", audio->getVolume());
  } else {
    debug("init sound... error!");
  }
}

void AudioDevice::onUpdate() { audio->loop(); }

void AudioDevice::onEnabled(bool enabled) {
  if (audio == nullptr) return;

  if (enabled == false) {
    audio->stopSong();
    audio->pauseResume();
  }
}