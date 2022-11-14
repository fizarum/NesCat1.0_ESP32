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
// for SPI flash memory access
#include "esp_spi_flash.h"

#define DEBUG true  // Serial debugging enable.

#include <audio.h>
#include <controls.h>
#include <utils.h>

// display part
#include <display.h>
#include <menu.h>

// SD card part
#include <storage.h>

// player Digital I/O used
//#define SD_CS          -1
// #define SPI_MOSI SOFTSD_MOSI_PIN
// #define SPI_MISO SOFTSD_MISO_PIN
// #define SPI_SCK SOFTSD_SCK_PIN
// #define I2S_DOUT I2S_DO_IO
// #define I2S_BCLK I2S_BCK_IO
// #define I2S_LRC I2S_WS_IO
//********************************************************************************

uint32_t psRAMSize = 0;
uint8_t *PSRAM;

// unsigned char *rom = 0;  // Actual ROM pointer

bool EXIT = false;

#define MAXFILES 512
char *filename[MAXFILES];

// NES EMU SECTION
#include "NESemulator_part1.h"
#include "NESemulator_part2.h"
#include "mappers.h"

void onKeysCallback(uint16_t keyMap) {
  if (keyMap == 0) {
    debug("on key release\n");
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
}

void onJoystickCallback(uint16_t joystickMoveMap) {
  debug("on joystick moved: %d\n", joystickMoveMap);
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
  getMemoryStatus();
  preparePsRam();

  controlsInit(&onKeysCallback, &onJoystickCallback);

  displayInit();

  initVideo();

  storageInit();
  printHomeFolder();

  install_timer(60);  // 60Hz
  getMemoryStatus();

  menuInit();
  nescreen::update();
}

uint8_t hJoyPos = 0;

void handleMenu() {
  if (JOY_LEFT) {
    hJoyPos = 1;
    JOY_LEFT = 0;
  } else if (JOY_RIGHT) {
    hJoyPos = 2;
    JOY_RIGHT = 0;
  } else {
    hJoyPos = 0;
  }
  updateActiveMenuIndex(hJoyPos);
  // todo: temporary solution - rework later
  if (hJoyPos != 0) {
    nescreen::update();
  }
}

void loop() {
  EXIT = false;
  controlsUpdate();
  handleMenu();
  switch (getCurrentMenuIndex()) {
    case 0:
      // debug("nes emulator init should be here");
      //   nes emulator part. code has been removed to save your mental health
      break;
    case 1:
      // debug("audio player init should be here");
      //   audio player part. code has been removed to save your mental health
      break;
    case 2:
      // debug("oscilloscope init should be here");
      //   oscilloscope part. code has been removed to save your mental health
      break;

    default:
      break;
  }
  JOY_SHARE = 0;
  JOY_OPTIONS = 0;
  EXIT = false;
}
