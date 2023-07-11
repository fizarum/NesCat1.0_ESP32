#include "app_audio_player.h"

#include <controls.h>
#include <display.h>
#include <utils.h>

bool AudioPlayer::handle(uint16_t keyState) {
  if (this->running == false) {
    return false;
  }

  if (isUpPressed(keyState)) {
    requestRedraw();
  }

  if (isCirclePressed(keyState)) {
    debug("audio player handles circle button - closing app");
    close();
  }
  return true;
}

void AudioPlayer::onUpdate() {
  //
}

void AudioPlayer::draw() {
  fillScreen(COLOR_VIOLET);
  drawString(64, 20, this->name, COLOR_WHITE);
  // todo: complete drawing content
}