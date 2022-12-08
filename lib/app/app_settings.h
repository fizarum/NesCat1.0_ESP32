#ifndef app_settings_h
#define app_settings_h

#include "app.h"

class Settings : public App {
 private:
  uint8_t selectBrightness = 1;
  uint8_t muteSound = 2;
  uint8_t powerSaveMode = 3;

 protected:
  void draw();
  void onUpdate();

 public:
  Settings() {
    this->id = userSettingsId;
    this->name = userSettingsTitle;
    this->running = false;
  }

  bool handle(uint16_t keyState);
};

#endif  // app_settings_h
