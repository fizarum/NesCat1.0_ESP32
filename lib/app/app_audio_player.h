#ifndef app_audio_player_h
#define app_audio_player_h

#include <app.h>

class AudioPlayer : public App {
 protected:
  void onDraw(DisplayDevice *display);
  void onUpdate();

 public:
  AudioPlayer() {
    this->id = audioPlayerId;
    this->name = audioPlayerTitle;
    this->running = false;
  }

  bool handle(JoystickDevice *joystick);
};

#endif  // app_audio_player_h