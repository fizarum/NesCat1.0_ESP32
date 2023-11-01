#include "app_audio_player.h"

#include <controls/controls.h>
#include <display.h>
#include <log.h>

void testSound();

bool AudioPlayer::handle(uint16_t keyState) {
  if (this->running == false) {
    return false;
  }

  if (isUpPressed()) {
    requestRedraw();
  }

  if (isBPressed()) {
    debug("audio player handles circle button - closing app");
    close();
  }

  if (isAPressed()) {
    testSound();
  }
  return true;
}

void AudioPlayer::onUpdate() {
  //
}

void AudioPlayer::draw() {
  fillScreen(COLOR_VIOLET);
  drawString(64, 20, this->name, COLOR_WHITE);
  drawString(10, 60, "press \"A\" to play");
  // todo: complete drawing content
}

// String ssid = "----";
// String password = "---";

void testSound() {
  debug("playing test sound...");
  //   WiFi.disconnect();
  //   WiFi.mode(WIFI_STA);
  //   WiFi.begin(ssid.c_str(), password.c_str());
  //   while (WiFi.status() != WL_CONNECTED) delay(1500);
  //   Serial.println("");
  //   Serial.println("WiFi connected");
  //   Serial.println("IP address: ");
  //   Serial.println(WiFi.localIP());
  //   Serial.println("");
  //   bool isSet = audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  //   debug("is sound set: %d", isSet);
  //   audio.setVolume(12);
  //   audio.connecttohost("0n-80s.radionetz.de:8000/0n-70s.mp3");
}