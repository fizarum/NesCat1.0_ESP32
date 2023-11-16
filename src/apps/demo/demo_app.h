#ifndef DEMO_APP_H
#define DEMO_APP_H
#include <app.h>

class DemoApp : public App {
 protected:
  void onOpen();

  void onDraw(DisplayDevice *display);
  void drawTitle(DisplayDevice *display){};

  bool onHandleInput(InputDevice *inputDevice);
  void onClose();
};

#endif  // DEMO_APP_H