#ifndef app_audio_player_h
#define app_audio_player_h

#include <app.h>

class AudioPlayer : public App {
 protected:
  void onDraw(DisplayDevice *display);
  void onUpdate();
  bool onHandleInput(JoystickDevice *joystick);

 public:
  AudioPlayer() {
    this->id = audioPlayerId;
    this->name = audioPlayerTitle;
    this->running = false;
  }
};

#endif  // app_audio_player_h