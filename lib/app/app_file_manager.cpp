#include "app_file_manager.h"

#include <controls.h>
#include <display.h>
#include <utils.h>

int selectedMenu = 0;

bool FileManager::handle(uint16_t keyState) {
  if (this->running == false) {
    return false;
  }

  if (isUpPressed(keyState)) {
    debug("fm handles up button");
    selectedMenu--;
    if (selectedMenu < 0) {
      selectedMenu = 3;
    }
    requestRedraw();
  }

  if (isDownPressed(keyState)) {
    debug("fm handles down button");
    selectedMenu++;
    if (selectedMenu > 3) {
      selectedMenu = 0;
    }
    requestRedraw();
  }

  if (isCirclePressed(keyState)) {
    debug("fm handles circle button - closing app");
    close();
  }

  return true;
}

void FileManager::onUpdate() {}

void FileManager::draw() {
  fillScreen(COLOR_ORANGE);
  drawString(64, 20, this->name, COLOR_WHITE);
  // todo: complete drawing content
}