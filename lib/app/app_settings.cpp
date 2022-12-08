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
  nescreen::fillScreen(GREEN_COLOR);
  nescreen::drawString(64, 10, this->name, WHITE_COLOR, GREEN_COLOR);
  // todo: complete drawing content
  nescreen::update();
}
