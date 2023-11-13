#ifndef app_settings_h
#define app_settings_h

#include <app.h>

class Settings : public App {
 private:
  uint8_t selectBrightness = 1;
  uint8_t muteSound = 2;
  uint8_t powerSaveMode = 3;

 protected:
  void onOpen();
  void onDraw(DisplayDevice *display);
  void drawBackground(DisplayDevice *display);
  void drawTitle(DisplayDevice *display);
  bool onHandleInput(InputDevice *inputDevice);
};

#endif  // app_settings_h