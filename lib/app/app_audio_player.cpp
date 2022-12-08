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
  nescreen::fillScreen(VIOLET_COLOR);
  nescreen::drawString(64, 10, this->name, WHITE_COLOR, VIOLET_COLOR);
  // todo: complete drawing content
  nescreen::update();
}