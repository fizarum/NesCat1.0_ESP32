#include "app_settings.h"

#include <ESP.h>
#include <device_manager.h>
#include <log.h>
#include <soc/rtc.h>

#include "devices/storage/storage_device.h"

char buff[24];
StorageDevice *_storage = nullptr;
uint8_t const firstOffset = 30;
uint8_t const secondOffset = 160;
uint8_t lineYPos = 50;

bool Settings::onHandleInput(InputDevice *inputDevice) { return false; }

void Settings::onOpen() {
  _storage = (StorageDevice *)DeviceManager::get(DEVICE_STORAGE_ID);
}

void Settings::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_DARKGREEN);
}

void Settings::drawTitle(DisplayDevice *display) {
  display->drawString(64, 20, getName());
}

void Settings::onDraw(DisplayDevice *display) {
  lineYPos = 50;

  display->drawString(firstOffset, lineYPos, "Model: ");
  display->drawString(secondOffset, lineYPos, ESP.getChipModel());
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "Revision: ");
  sprintf(buff, "%d", ESP.getChipRevision());
  display->drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "Cores: ");
  sprintf(buff, "%d", ESP.getChipCores());
  display->drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  // Get current CPU clock configuration
  rtc_cpu_freq_config_t conf;
  rtc_clk_cpu_freq_get_config(&conf);
  display->drawString(firstOffset, lineYPos, "CPU Speed: ");
  sprintf(buff, "%d Mhz", conf.freq_mhz);
  display->drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "Flash size: ");
  sprintf(buff, "%d MB", ESP.getFlashChipSize() / (1024 * 1024));
  display->drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  display->drawString(firstOffset, lineYPos, "RAM: ");
  sprintf(buff, "%d/%d Kb", ESP.getFreeHeap() / 1024, ESP.getHeapSize() / 1024);
  display->drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  if (_storage != nullptr) {
    uint32_t total = _storage->totalMBytes();
    uint32_t used = _storage->usedMBytes();
    float occupied = (float)used / (float)total;

    display->drawString(firstOffset, lineYPos, "SD size: ");
    sprintf(buff, "%u Mb", total);
    display->drawString(secondOffset, lineYPos, buff);
    lineYPos += 20;

    display->drawString(firstOffset, lineYPos, "SD used: ");
    sprintf(buff, "%2.2f %%", occupied);
    display->drawString(secondOffset, lineYPos, buff);
  }
}
