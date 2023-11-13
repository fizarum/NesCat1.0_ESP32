#ifndef app_file_manager_h
#define app_file_manager_h

#include <app.h>

class FileManager : public App {
 private:
  uint16_t cursorPosOnScreen;
  int8_t cursorPos = 0;

 protected:
  void onOpen();
  void onDraw(DisplayDevice *display);
  void drawBackground(DisplayDevice *display);
  void drawTitle(DisplayDevice *display);
  bool onHandleInput(InputDevice *inputDevice);
  void onClose();
};

#endif  // app_file_manager_h