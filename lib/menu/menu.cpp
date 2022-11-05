#include "menu.h"

#include <display.h>
#include <utils.h>

const int8_t MAX_MENU_ITEMS = 4;
const int8_t LAST_ITEM_INDEX = MAX_MENU_ITEMS - 1;

void drawCurentMenuItem();

int8_t menuItem = 0;
// 0 menu index
const char *const emulator = "NES Emulator";
// 1 menu index
const char *const audioPlayer = "Audio Player";
// 2 menu index
const char *const oscilloscope = "Oscilloscope";
// 3 menu index
const char *const filemanager = "File Manager";

// position of menu
const uint8_t xPos = 64;
const uint8_t yPos = V_CENTER - 24;

void menuInit() { drawCurentMenuItem(); }

void updateActiveMenuIndex(uint8_t isJoyMovedHorizontally) {
  switch (isJoyMovedHorizontally) {
    // left movement
    case 1: {
      menuItem--;
      if (menuItem < 0) menuItem = LAST_ITEM_INDEX;
      break;
    }
    // right movement
    case 2: {
      menuItem++;
      if (menuItem > LAST_ITEM_INDEX) menuItem = 0;
      break;
    }
    default:
      return;
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
  drawCurentMenuItem();
}

const char *getCurrentMenuTitle() {
  switch (menuItem) {
    case 0:
      return emulator;

    case 1:
      return audioPlayer;

    case 2:
      return oscilloscope;

    case 3:
      return filemanager;

    default:
      return "";
  }
}

uint8_t getCurrentMenuIndex() { return (uint8_t)menuItem; }

void drawCurentMenuItem() {
  nescreen::fillScreen(MENU_BACKGROUND_COLOR);
  nescreen::drawString(xPos, yPos, getCurrentMenuTitle(), MENU_TEXT_COLOR,
                       MENU_BACKGROUND_COLOR);
  debug("menu item: %s", getCurrentMenuTitle());
}