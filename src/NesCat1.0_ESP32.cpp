//********************************************************************************
//*
//*   NCAT SYSTEM Rework 1.1
//*
//*   Requirements: NodeMCU ESP32S, ILI9341 LCD, MICRO SD CARD slot,
//*   PCM5102 I2S AUDIO MODULE, OTHER PARTS...
//*
//*   Only for personal & educational use!
//*   (GPL-3.0 License)
//*
//********************************************************************************
//*
//*   features:
//*   - ESP32-WROVER PSRAM module support
//*   - MicroSD card support
//*   - 320*240 2.8" LCD ILI9341 display
//*   - Composite TV OUT Video PAL
//*   - I2S AUDIO support PCM5102 module
//*   - huge NES ROMs up to 512kB (read from FLASH)
//*
//********************************************************************************
#include <Arduino.h>
#include <Wire.h>
// for SPI flash memory access
#include <esp_spi_flash.h>

// Serial debugging enable.
#define DEBUG true

#include <audio.h>
#include <controls.h>
#include <utils.h>

// display part
#include <display.h>

// SD card part
#include <storage.h>
// start app - menu
#include <app_menu.h>

Menu menu;

uint32_t psRAMSize = 0;
uint8_t *PSRAM;

// bool EXIT = false;

void onAppClosedCallback();

void onKeysCallback(uint16_t keyMap) {
  if (menu.handle(keyMap) == true) {
    return;
  }

  if (keyMap == 0) {
    debug("on key release");
    return;
  }

  if (isTrianglePressed(keyMap)) {
    debug("triangle pressed");
  }

  if (isCirclePressed(keyMap)) {
    debug("circle pressed");
  }

  if (isCrossPressed(keyMap)) {
    debug("cross pressed");
  }

  if (isSquarePressed(keyMap)) {
    debug("square pressed");
  }

  if (isSelectPressed(keyMap)) {
    debug("select pressed");
  }
  if (isStartPressed(keyMap)) {
    debug("start pressed");
  }
  nescreen::update();
}

void onJoystickCallback(uint16_t joystickMoveMap) {
  menu.handle(joystickMoveMap);
}

void preparePsRam() {
  psRAMSize = ESP.getPsramSize();
  getPsRamStatus(psRAMSize);

  if (psRAMSize > 0) {
    PSRAM = (uint8_t *)ps_malloc(2097152);  // PSRAM malloc 2MB
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial completion init
  }
  controlsInit(&onKeysCallback, &onJoystickCallback);

  // getMemoryStatus();
  preparePsRam();

  displayInit();

  initVideo();

  // install_timer(60);  // 60Hz
  // getMemoryStatus();

  menu.open(&onAppClosedCallback);
}

void loop() {
  // EXIT = false;
  controlsUpdate();
  menu.update();
  // JOY_SHARE = 0;
  // JOY_OPTIONS = 0;
  // EXIT = false;
}

void onAppClosedCallback() { menu.closeUserApp(); }