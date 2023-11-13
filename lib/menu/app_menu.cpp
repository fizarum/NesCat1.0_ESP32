#include "app_menu.h"

#include <log.h>

#define DISPLAY_H_CENTER DISPLAY_WIDTH / 2
#define DISPLAY_V_CENTER DISPLAY_HEIGHT / 2

void (*onMenuSelectedCallback)(uint8_t appId);

void Menu::setAppData(std::vector<App *> *apps) {
  if (apps == nullptr || apps->empty() == true) return;

  for (auto it = apps->begin(); it != apps->end(); ++it) {
    App *app = *it;
    MenuItem *menuItem = new MenuItem(app->getId(), app->getName());
    _items.push_back(menuItem);
  }
}

void Menu::onOpen() {
  this->totalItems = _items.size();
  this->selectedMenu = 0;
}

bool Menu::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isLeftPressed()) {
    debug("menu: left key pressed: %u", inputDevice->keysState());
    this->selectedMenu -= 1;
    if (this->selectedMenu < 0) {
      this->selectedMenu = totalItems - 1;
    };
    requestRedraw();
    return true;
  }
  if (inputDevice->isRightPressed()) {
    debug("menu: right key pressed: %u", inputDevice->keysState());
    this->selectedMenu += 1;
    if (this->selectedMenu >= totalItems) {
      this->selectedMenu = 0;
    }
    requestRedraw();
    return true;
  }

  if (inputDevice->isXPressed()) {
    debug("menu: is cross key pressed: %u", inputDevice->keysState());
    uint8_t appId = _items[selectedMenu]->id;
    onMenuSelectedCallback(appId);
    return true;
  }

  return false;
}

void Menu::onDraw(DisplayDevice *display) {
  MenuItem *item = _items[selectedMenu];
  display->drawString(DISPLAY_H_CENTER, DISPLAY_V_CENTER, item->title,
                      COLOR_WHITE, MC_DATUM);
  debug("menu item: %s", item->title);
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