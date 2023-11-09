#include "app_menu.h"

#include <log.h>

#include "app_audio_player.h"
#include "app_file_manager.h"
#include "app_settings.h"
#include "nes_launcher.h"

FileManager fileManager;
Settings settings;
AudioPlayer audioPlayer;
NesLauncher nesLauncher;

App *app = nullptr;

uint8_t totalMenuItems = 4;

// menu items
MenuItem menuItems[] = {
    MenuItem(fileManagerId, fileManagerTitle),
    MenuItem(nesEmulatorId, emulatorTitle),
    MenuItem(audioPlayerId, audioPlayerTitle),
    MenuItem(userSettingsId, userSettingsTitle),
};

/// function declarations
App *pickSelectedApp(uint8_t menuIndex);

bool isUserAppActive() { return app != nullptr && app->isRunning(); }

void Menu::init() {
  this->items = menuItems;
  this->totalItems = totalMenuItems;
  this->selectedMenu = 0;
}

bool Menu::onHandleInput(JoystickDevice *joystick) {
  if (isUserAppActive() == true && app->handleInput(joystick) == true) {
    return true;
  }

  if (joystick->isLeftPressed()) {
    debug("menu: left key pressed: %u", joystick->keysState());
    this->selectedMenu -= 1;
    if (this->selectedMenu < 0) {
      this->selectedMenu = totalMenuItems - 1;
    };
    requestRedraw();
    return true;
  }
  if (joystick->isRightPressed()) {
    debug("menu: right key pressed: %u", joystick->keysState());
    this->selectedMenu += 1;
    if (this->selectedMenu >= totalMenuItems) {
      this->selectedMenu = 0;
    }
    requestRedraw();
    return true;
  }

  if (joystick->isXPressed()) {
    debug("menu: is cross key pressed: %u", joystick->keysState());
    app = pickSelectedApp(selectedMenu);
    if (app != nullptr) {
      debug("menu: opening %s", app->getName());
      app->open(this->onCloseListener);
    }
    requestRedraw();
    return true;
  }

  return false;
}

void Menu::onUpdate() {
  if (isUserAppActive() == true) {
    if (needsToBeRedrawn() == true) {
      requestRedraw();
    }

    app->update();
  } else {
    // update menu...
  }
}

void Menu::onDraw(DisplayDevice *display) {
  if (isUserAppActive() == true) {
    app->draw(display);
  } else {
    MenuItem item = menuItems[selectedMenu];
    display->drawString(H_CENTER, V_CENTER, item.title, COLOR_WHITE, MC_DATUM);
    debug("menu item: %s", item.title);
  }
}

void Menu::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_BLUE);
}
void Menu::drawTitle(DisplayDevice *display) {
  display->drawString(10, 10, "**debug ver**", COLOR_LIGHTGREY);
  display->drawString(190, 10, "78%", COLOR_LIGHTGREY);
  // time & battery info here
  display->drawString(250, 10, "22:01", COLOR_LIGHTGREY);
}

void Menu::drawStatusBar(DisplayDevice *display) {
  // some nav hints here
  display->drawString(20, 210, "use < > to navigate", COLOR_LIGHTGREY);
}

void Menu::closeUserApp() {
  debug("app is closed, going to menu");
  app = nullptr;
  requestRedraw();
}

bool Menu::needsToBeRedrawn() {
  if (isUserAppActive() == true) {
    return app->needsToBeRedrawn();
  } else {
    return this->needsToRedraw;
  }
}

App *pickSelectedApp(uint8_t menuIndex) {
  switch (menuIndex) {
    case 0:
      return &fileManager;
    case 1:
      return &nesLauncher;
    case 2:
      return &audioPlayer;
    case 3:
      return &settings;
    default:
      return nullptr;
  }
}
