#include "app_menu.h"

#include <log.h>

uint8_t totalMenuItems = 4;

// menu items
MenuItem menuItems[] = {
    MenuItem(fileManagerId, fileManagerTitle),
    MenuItem(nesEmulatorId, emulatorTitle),
    MenuItem(audioPlayerId, audioPlayerTitle),
    MenuItem(userSettingsId, userSettingsTitle),
};

void (*onMenuSelectedCallback)(uint8_t appId);
uint8_t getAppIdByPosition(uint8_t menuIndex);

void Menu::init() {
  this->items = menuItems;
  this->totalItems = totalMenuItems;
  this->selectedMenu = 0;
}

bool Menu::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isLeftPressed()) {
    debug("menu: left key pressed: %u", inputDevice->keysState());
    this->selectedMenu -= 1;
    if (this->selectedMenu < 0) {
      this->selectedMenu = totalMenuItems - 1;
    };
    requestRedraw();
    return true;
  }
  if (inputDevice->isRightPressed()) {
    debug("menu: right key pressed: %u", inputDevice->keysState());
    this->selectedMenu += 1;
    if (this->selectedMenu >= totalMenuItems) {
      this->selectedMenu = 0;
    }
    requestRedraw();
    return true;
  }

  if (inputDevice->isXPressed()) {
    debug("menu: is cross key pressed: %u", inputDevice->keysState());
    uint8_t appId = getAppIdByPosition(selectedMenu);
    onMenuSelectedCallback(appId);
    return true;
  }

  return false;
}

void Menu::onDraw(DisplayDevice *display) {
  MenuItem item = menuItems[selectedMenu];
  display->drawString(H_CENTER, V_CENTER, item.title, COLOR_WHITE, MC_DATUM);
  debug("menu item: %s", item.title);
}

void Menu::attachOnMenuSelectedCallback(void (*callback)(uint8_t)) {
  onMenuSelectedCallback = callback;
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

uint8_t getAppIdByPosition(uint8_t menuIndex) {
  switch (menuIndex) {
    case 0:
      return fileManagerId;
    case 1:
      return nesEmulatorId;
    case 2:
      return audioPlayerId;
    case 3:
      return userSettingsId;
    default:
      return menuId;
  }
}
