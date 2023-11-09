#include "app_settings.h"

#include <ESP.h>
#include <log.h>
#include <soc/rtc.h>

#include "../device/controls/joystick_device.h"
#include "../device/device_manager.h"
#include "../device/storage/storage_device.h"

char buff[24];

bool Settings::handle(JoystickDevice *joystick) {
  if (this->running == false) {
    return false;
  }

  if (joystick->isBPressed()) {
    debug("settings handles circle button - closing app");
    close();
  }
  return true;
}

void Settings::onUpdate() {
  //
}

uint8_t const firstOffset = 30;
uint8_t const secondOffset = 160;
uint8_t lineYPos = 50;

void Settings::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_DARKGREEN);
}

void Settings::drawTitle(DisplayDevice *display) {
  display->drawString(64, 20, this->name);
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

  StorageDevice *storage = (StorageDevice *)DeviceManager::get(STORAGE_DEVICE);
  if (storage != nullptr) {
    uint32_t total = storage->totalMBytes();
    uint32_t used = storage->usedMBytes();
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
