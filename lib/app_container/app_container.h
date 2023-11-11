#ifndef APP_CONTAINER_H
#define APP_CONTAINER_H

#include <app.h>
#include <app_menu.h>

#include <vector>

#include "../device/controls/joystick_device.h"
#include "../device/display/display_device.h"

class AppContainer {
 private:
  std::vector<App *> apps;
  Menu *menuApp = nullptr;
  App *activeApp = nullptr;

 public:
  void init();
  void start();
  void update();
  void draw(DisplayDevice *display);
  void handleInput(JoystickDevice *joystick);
  App *getAppById(uint8_t appId);
  void selectApp(App *app);
  void selectMenuApp();
};

#endif  // APP_CONTAINER_H