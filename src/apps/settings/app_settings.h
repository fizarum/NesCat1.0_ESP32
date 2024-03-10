#ifndef app_settings_h
#define app_settings_h

#include <app.h>

#include "devices/storage/storage_device.h"
class Settings : public App {
 private:
  uint8_t selectBrightness = 1;
  uint8_t muteSound = 2;
  uint8_t powerSaveMode = 3;

  char buff[24];

  StorageDevice *_storage = nullptr;
  DisplayDevice *_display = nullptr;

  void increaseDisplayBacklight(uint8_t step);
  void decreaseDisplayBacklight(uint8_t step);

 protected:
  void onOpen();
  void onDraw(DisplayDevice *display);
  void drawBackground(DisplayDevice *display);
  void drawTitle(DisplayDevice *display);
  bool onHandleInput(InputDevice *inputDevice);
};

#endif  // app_settings_h
