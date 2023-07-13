#ifndef app_menu_h
#define app_menu_h

#include "app.h"

class MenuItem {
 public:
  uint16_t id;
  const char *title;

  MenuItem(uint16_t id, const char *title) {
    this->id = id;
    this->title = title;
  }
};

class Menu : public App {
 private:
  MenuItem *items;
  uint8_t totalItems;
  int8_t selectedMenu;

  void init();

 protected:
  void drawBackground();
  void drawTitle();
  void drawStatusBar();
  void draw();

 public:
  Menu() {
    this->id = menuId;
    this->name = menuTitle;
    this->running = false;
  }

  bool handle(uint16_t keyState);
  void onUpdate();
  void closeUserApp();
  bool needsToBeRedrawn();
};

#endif  // app_menu_h
