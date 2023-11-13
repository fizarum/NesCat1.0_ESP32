#ifndef APP_CONTAINER_H
#define APP_CONTAINER_H

#include <app.h>
#include <app_menu.h>
#include <display_device/display_device.h>
#include <input_device/input_device.h>

#include <vector>

class AppLauncher {
 private:
  std::vector<App *> _apps;
  Menu _menuApp;
  App *_activeApp = nullptr;

 public:
  void init();

  /**
   * @brief add user app to the app launcher
   *
   * @param app user application
   * @param appName new application's name
   */
  void add(App *app, const char *appName);
  void start();
  void update();
  void draw(DisplayDevice *display);
  void handleInput(InputDevice *inputDevice);
  App *getAppById(uint8_t appId);
  void selectApp(App *app);
  void selectMenuApp();
};

#endif  // APP_CONTAINER_H