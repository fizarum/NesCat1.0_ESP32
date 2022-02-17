#include "menu.h"

const int8_t MAX_MENU_ITEMS = 3;
const int8_t LAST_ITEM_INDEX = MAX_MENU_ITEMS - 1;

int8_t menuItem = 0;
// 0 menu index
const char *const emulator = " NES Emulator ";
// 1 menu index
const char *const audioPlayer = " Audio Player ";
// 2 menu index
const char *const oscilloscope = " Oscilloscope ";

void updateActiveMenuIndex(uint8_t isJoyMovedHorizontally) {
  switch (isJoyMovedHorizontally) {
    // left movement
    case 1: {
      menuItem--;
      if (menuItem < 0) menuItem = 2;
      break;
    }
    // right movement
    case 2: {
      menuItem++;
      if (menuItem > 2) menuItem = 0;
      break;
    }
    default:
      break;
  }
  if (menuItem < 0) {
    menuItem = 0;
  }
  if (menuItem >= MAX_MENU_ITEMS) {
    menuItem = LAST_ITEM_INDEX;
  }

  // if ((JOY_SQUARE == 1 || JOY_SHARE == 1) && JOY_OPTIONS == 0) {
  //   JOY_SQUARE = 0;
  //   JOY_SHARE = 0;
  //   MenuItem++;
  //   if (MenuItem > 2) MenuItem = 0;
  // }
  // return MenuItem;
}

const char *getCurrentMenuTitle() {
  switch (menuItem) {
    case 0:
      return emulator;
    case 1:
      return audioPlayer;

    case 2:
      return oscilloscope;
    default:
      break;
  }
}

uint8_t getCurrentMenuIndex() { return (uint8_t)menuItem; }
