#include "app_settings.h"

#include <ESP.h>
#include <device_manager.h>
#include <log.h>
#include <soc/rtc.h>

uint8_t const firstOffset = 30;
uint8_t const secondOffset = 160;
uint8_t lineYPos = 50;

bool Settings::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isUpKeyUp()) {
    increaseDisplayBacklight(5);
  }
  if (inputDevice->isDownKeyUp()) {
    decreaseDisplayBacklight(5);
  }
  return true;
}

void Settings::onOpen() {
  this->_storage = (StorageDevice *)DeviceManager::get(DEVICE_STORAGE_ID);
}

void Settings::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_DARKGREEN);
}

void Settings::drawTitle(DisplayDevice *display) {
  display->drawString(64, 20, getName());
}

void Settings::onDraw(DisplayDevice *display) {
  if (this->_display == nullptr) {
    this->_display = display;
  }

  lineYPos = 50;

  display->drawString(firstOffset, lineYPos, "Model: ");
  display->drawString(secondOffset, lineYPos, ESP.getChipModel());
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "Revision: ");
  sprintf(this->buff, "%d", ESP.getChipRevision());
  display->drawString(secondOffset, lineYPos, this->buff);
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "Cores: ");
  sprintf(this->buff, "%d", ESP.getChipCores());
  display->drawString(secondOffset, lineYPos, this->buff);
  lineYPos += 20;

  // Get current CPU clock configuration
  rtc_cpu_freq_config_t conf;
  rtc_clk_cpu_freq_get_config(&conf);
  display->drawString(firstOffset, lineYPos, "CPU Speed: ");
  sprintf(this->buff, "%d Mhz", conf.freq_mhz);
  display->drawString(secondOffset, lineYPos, this->buff);
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "Flash size: ");
  sprintf(this->buff, "%d MB", ESP.getFlashChipSize() / (1024 * 1024));
  display->drawString(secondOffset, lineYPos, this->buff);
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "RAM: ");
  sprintf(this->buff, "%d/%d Kb", ESP.getFreeHeap() / 1024,
          ESP.getHeapSize() / 1024);
  display->drawString(secondOffset, lineYPos, this->buff);
  lineYPos += 20;

  if (_storage != nullptr) {
    uint32_t total = _storage->totalMBytes();
    uint32_t used = _storage->usedMBytes();
    float occupied = (float)used / (float)total;

    display->drawString(firstOffset, lineYPos, "SD size: ");
    sprintf(this->buff, "%u Mb", total);
    display->drawString(secondOffset, lineYPos, this->buff);
    lineYPos += 20;

    display->drawString(firstOffset, lineYPos, "SD used: ");
    sprintf(this->buff, "%2.2f %%", occupied);
    display->drawString(secondOffset, lineYPos, this->buff);
  }
}

void Settings::increaseDisplayBacklight(uint8_t step) {
  uint8_t value = this->_display->getBacklightValue();
  if (value > BL_LEVEL_MAX - step) {
    value = BL_LEVEL_MAX;
  } else {
    value += step;
  }
  this->_display->setBackLightValue(value);
}

void Settings::decreaseDisplayBacklight(uint8_t step) {
  uint8_t value = this->_display->getBacklightValue();
  if (value < BL_LEVEL_MIN + step) {
    value = BL_LEVEL_MIN;
  } else {
    value -= step;
  }
  this->_display->setBackLightValue(value);
}