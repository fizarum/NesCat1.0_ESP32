#include "app_audio_player.h"

#include <log.h>

void testSound();

bool AudioPlayer::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isAPressed()) {
    testSound();
  }
  return true;
}

void AudioPlayer::onDraw(DisplayDevice *display) {
  display->drawString(10, 60, "press \"A\" to play");
  // TODO: complete drawing content
}

void AudioPlayer::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_VIOLET);
}

void AudioPlayer::drawTitle(DisplayDevice *display) {
  display->drawString(64, 20, getName(), COLOR_WHITE);
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