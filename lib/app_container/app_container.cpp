#include "app_container.h"

#include <log.h>

#include "../app/app_audio_player.h"
#include "../app/app_file_manager.h"
#include "../app/app_menu.h"
#include "../app/app_settings.h"
#include "../app/nes_launcher.h"

void onAppClosedCallback();
void onMenuSelectedCallback(uint8_t appId);

AppContainer *_appContainer = nullptr;

void AppContainer::init() {
  menuApp = new Menu();
  menuApp->attachOnMenuSelectedCallback(&onMenuSelectedCallback);
  apps = {
      new FileManager(),
      new Settings(),
      new AudioPlayer(),
      new NesLauncher(),
  };
  // select menu app as startup one
  this->activeApp = menuApp;
  _appContainer = this;
};

void AppContainer::start() { this->activeApp->open(&onAppClosedCallback); }

void AppContainer::update() { this->activeApp->update(); }
void AppContainer::draw(DisplayDevice *display) {
  this->activeApp->draw(display);
}
void AppContainer::handleInput(JoystickDevice *joystick) {
  this->activeApp->handleInput(joystick);
}

App *AppContainer::getAppById(uint8_t appId) {
  for (uint8_t index = 0; index < apps.size(); ++index) {
    App *app = apps[index];
    if (app->getId() == appId) {
      return app;
    }
  }
  return nullptr;
}

void AppContainer::selectApp(App *app) {
  if (app == nullptr) return;
  this->activeApp = app;
  app->open(&onAppClosedCallback);
  app->requestRedraw();
}

void AppContainer::selectMenuApp() { this->selectApp(this->menuApp); }

void onAppClosedCallback() {
  debug("on app close called");
  _appContainer->selectMenuApp();
}

void onMenuSelectedCallback(uint8_t appId) {
  App *app = _appContainer->getAppById(appId);
  if (app == nullptr) return;
  _appContainer->selectApp(app);
}