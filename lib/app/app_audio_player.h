#ifndef app_audio_player_h
#define app_audio_player_h

#include <app.h>

class AudioPlayer : public App {
 protected:
  void draw();
  void onUpdate();

 public:
  AudioPlayer() {
    this->id = audioPlayerId;
    this->name = audioPlayerTitle;
    this->running = false;
  }

  bool handle(uint16_t keyState);
};

#endif  // app_audio_player_h