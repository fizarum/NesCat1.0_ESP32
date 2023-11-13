#include "app_launcher.h"

#include <log.h>

#include "apps_registry.h"

void onAppClosedCallback();
void onMenuSelectedCallback(uint8_t appId);

AppLauncher *_appContainer = nullptr;

void AppLauncher::init() {
  _menuApp.init(getNewId(), "Menu");
  _menuApp.attachOnMenuSelectedCallback(&onMenuSelectedCallback);

  // select menu app as startup one
  _activeApp = &_menuApp;
  _appContainer = this;
};

void AppLauncher::add(App *app, const char *appName) {
  app->init(getNewId(), appName);
  _apps.push_back(app);
}

void AppLauncher::start() {
  // app launcher always starts from menu
  _menuApp.setAppData(&_apps);
  _menuApp.open(&onAppClosedCallback);
}

void AppLauncher::update() { _activeApp->update(); }
void AppLauncher::draw(DisplayDevice *display) { _activeApp->draw(display); }
void AppLauncher::handleInput(InputDevice *inputDevice) {
  _activeApp->handleInput(inputDevice);
}

App *AppLauncher::getAppById(uint8_t appId) {
  for (uint8_t index = 0; index < _apps.size(); ++index) {
    App *app = _apps[index];
    if (app->getId() == appId) {
      return app;
    }
  }
  return nullptr;
}

void AppLauncher::selectApp(App *app) {
  if (app == nullptr) return;
  _activeApp = app;
  app->open(&onAppClosedCallback);
  app->requestRedraw();
}

void AppLauncher::selectMenuApp() { this->selectApp(&_menuApp); }

void onAppClosedCallback() {
  debug("on app close called");
  _appContainer->selectMenuApp();
}

void onMenuSelectedCallback(uint8_t appId) {
  App *app = _appContainer->getAppById(appId);
  if (app == nullptr) return;
  _appContainer->selectApp(app);
}