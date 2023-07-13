#include "app_settings.h"

#include <ESP.h>
#include <controls.h>
#include <display.h>
#include <utils.h>

#include "soc/rtc.h"

char buff[24];

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

uint8_t const firstOffset = 30;
uint8_t const secondOffset = 160;
uint8_t lineYPos = 50;

void Settings::drawBackground() { fillScreen(COLOR_DARKGREEN); }

void Settings::drawTitle() { drawString(64, 20, this->name); }

void Settings::draw() {
  lineYPos = 50;

  drawString(firstOffset, lineYPos, "Model: ");
  drawString(secondOffset, lineYPos, ESP.getChipModel());
  lineYPos += 20;

  drawString(firstOffset, lineYPos, "Revision: ");
  sprintf(buff, "%d", ESP.getChipRevision());
  drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  drawString(firstOffset, lineYPos, "Cores: ");
  sprintf(buff, "%d", ESP.getChipCores());
  drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  // Get current CPU clock configuration
  rtc_cpu_freq_config_t conf;
  rtc_clk_cpu_freq_get_config(&conf);
  drawString(firstOffset, lineYPos, "CPU Speed: ");
  sprintf(buff, "%d Mhz", conf.freq_mhz);
  drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  drawString(firstOffset, lineYPos, "Flash size: ");
  sprintf(buff, "%d MB", ESP.getFlashChipSize() / (1024 * 1024));
  drawString(secondOffset, lineYPos, buff);
  lineYPos += 20;

  drawString(firstOffset, lineYPos, "RAM: ");
  sprintf(buff, "%d/%d Kb", ESP.getFreeHeap() / 1024, ESP.getHeapSize() / 1024);
  drawString(secondOffset, lineYPos, buff);
}
