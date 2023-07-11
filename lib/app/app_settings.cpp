#include "app_settings.h"

#include <controls.h>
#include <display.h>
#include <utils.h>

bool Settings::handle(uint16_t keyState) {
  if (this->running == false) {
    return false;
  }

  if (isCirclePressed(keyState)) {
    debug("settings handles circle button - closing app");
    close();
  }
  return true;
}

void Settings::onUpdate() {
  //
}

void Settings::draw() {
  fillScreen(COLOR_GREEN);
  drawString(64, 20, this->name, COLOR_WHITE);
  // todo: complete drawing content
}
