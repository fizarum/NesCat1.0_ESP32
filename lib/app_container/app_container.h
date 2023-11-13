#ifndef APP_CONTAINER_H
#define APP_CONTAINER_H

#include <app.h>
#include <app_menu.h>
#include <display_device/display_device.h>
#include <input_device/input_device.h>

#include <vector>

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
  void handleInput(InputDevice *inputDevice);
  App *getAppById(uint8_t appId);
  void selectApp(App *app);
  void selectMenuApp();
};

#endif  // APP_CONTAINER_H