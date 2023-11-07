#include "app_file_manager.h"

#include <log.h>

#include "../device/controls/joystick_device.h"

int selectedMenu = 0;

bool FileManager::handle(JoystickDevice *joystick) {
  if (this->running == false) {
    return false;
  }

  if (joystick->isUpPressed()) {
    debug("fm handles up button");
    selectedMenu--;
    if (selectedMenu < 0) {
      selectedMenu = 3;
    }
    requestRedraw();
  }

  if (joystick->isDownPressed()) {
    debug("fm handles down button");
    selectedMenu++;
    if (selectedMenu > 3) {
      selectedMenu = 0;
    }
    requestRedraw();
  }

  if (joystick->isBPressed()) {
    debug("fm handles circle button - closing app");
    close();
  }

  return true;
}

void FileManager::onUpdate() {}

void FileManager::onDraw(DisplayDevice *display) {
  display->fillScreen(COLOR_ORANGE);
  display->drawString(64, 20, this->name, COLOR_WHITE);
  // TODO: complete drawing content
}