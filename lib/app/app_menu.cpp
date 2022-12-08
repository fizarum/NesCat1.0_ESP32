#include "app_menu.h"

#include <controls.h>

#include "app_audio_player.h"
#include "app_file_manager.h"
#include "app_settings.h"
#include "display.h"
#include "utils.h"

// position of menu
const uint8_t xPos = 64;
const uint8_t yPos = V_CENTER - 24;

FileManager fileManager;
Settings settings;
AudioPlayer audioPlayer;

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
void _menuDraw(MenuItem *item);
App *pickSelectedApp(uint8_t menuIndex);
void onUserAppClosedCallback();

bool isUserAppActive() { return app != nullptr && app->isRunning(); }

void Menu::init() {
  this->items = menuItems;
  this->totalItems = totalMenuItems;
  this->selectedMenu = 0;
}

void Menu::draw() {
  if (isUserAppActive() == true) {
    app->draw();
  } else {
    MenuItem item = menuItems[selectedMenu];
    _menuDraw(&item);
  }
}

bool Menu::handle(uint16_t keyState) {
  if (isUserAppActive() == true && app->handle(keyState) == true) {
    return true;
  }

  if (isLeftPressed(keyState)) {
    debug("menu: left key pressed: %u", keyState);
    this->selectedMenu -= 1;
    if (this->selectedMenu < 0) {
      this->selectedMenu = totalMenuItems - 1;
    };
    requestRedraw();
    return true;
  }
  if (isRightPressed(keyState)) {
    debug("menu: right key pressed: %u", keyState);
    this->selectedMenu += 1;
    if (this->selectedMenu >= totalMenuItems) {
      this->selectedMenu = 0;
    }
    requestRedraw();
    return true;
  }

  if (isCrossPressed(keyState)) {
    debug("menu: is cross key pressed: %u", keyState);
    app = pickSelectedApp(selectedMenu);
    if (app != nullptr) {
      app->open(this->onCloseListener);
      debug("menu: opening %s", app->getName());
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

void _menuDraw(MenuItem *item) {
  nescreen::fillScreen(BLUE_COLOR);
  nescreen::drawString(xPos, yPos, item->title, WHITE_COLOR, BLUE_COLOR);
  debug("menu item: %s", item->title);
  nescreen::update();
}

App *pickSelectedApp(uint8_t menuIndex) {
  switch (menuIndex) {
    case 0:
      return &fileManager;
    case 2:
      return &audioPlayer;
    case 3:
      return &settings;
    default:
      return nullptr;
  }
}
