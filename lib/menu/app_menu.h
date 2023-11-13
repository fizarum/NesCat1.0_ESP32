#ifndef app_menu_h
#define app_menu_h

#include <app.h>

#include <vector>

class MenuItem {
 public:
  uint32_t id;
  const char *title;

  MenuItem(uint32_t id, const char *title) {
    this->id = id;
    this->title = title;
  }
};

class Menu : public App {
 private:
  std::vector<MenuItem *> _items;
  uint8_t totalItems;
  int8_t selectedMenu;

 protected:
  void onOpen();
  void drawBackground(DisplayDevice *display);
  void drawTitle(DisplayDevice *display);
  void drawStatusBar(DisplayDevice *display);
  void onDraw(DisplayDevice *display);

  bool preventClosingByUser() { return true; }
  bool onHandleInput(InputDevice *inputDevice);

 public:
  // callback has uint8_t parameter - application id
  void attachOnMenuSelectedCallback(void (*callback)(uint8_t));

  // provide list of applications so menu can read their names and ids
  // names are used to show on menu
  // ids are used to launch apps from menu
  void setAppData(std::vector<App *> *apps);
};

#endif  // app_menu_h
