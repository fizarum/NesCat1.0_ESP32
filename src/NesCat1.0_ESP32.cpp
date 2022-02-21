//********************************************************************************
//*
//*   NCAT SYSTEM Rework 1.1
//*
//*   Includes: 1. NES EMULATOR
//*
//*   Requirements: NodeMCU ESP32S, ILI9341 LCD, MICROSD CARD slot,
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

#define COMPOSITE_VIDEO_ENABLED true  // Do not disable! it also disable ADC.

#define DEBUG true        // Serial debugging enable.
#define DEBUGEXTRA false  // Extra Serial debugging enable.

#include <audio.h>
#include <controls.h>

// display part
#include <display.h>
#include <menu.h>

// SD card part
#include <fstorage.h>

// player Digital I/O used
//#define SD_CS          -1
// #define SPI_MOSI SOFTSD_MOSI_PIN
// #define SPI_MISO SOFTSD_MISO_PIN
// #define SPI_SCK SOFTSD_SCK_PIN
// #define I2S_DOUT I2S_DO_IO
// #define I2S_BCLK I2S_BCK_IO
// #define I2S_LRC I2S_WS_IO
//********************************************************************************
// MAIN LIBRARIES:

#include <Arduino.h>
#include <esp_task_wdt.h>
#include <utils.h>

static void videoTask(void *arg);

//********************************************************************************
// VARIABLES:

uint32_t psRAMSize = 0;
uint8_t *PSRAM;

//--------------------------------------------------------------------------------
// SPI FLASH MEMORY ACCESS:
#include "esp_spi_flash.h"

unsigned char *rom = 0;  // Actual ROM pointer

// COMPOSITE_VIDEO_OUT
#if COMPOSITE_VIDEO_ENABLED
#include "compositevideo.h"
#endif

//===============================================================================
// VIDEO SYSTEM:
QueueHandle_t vidQueue;

inline void updateScreen() { xQueueSend(vidQueue, &screenMemory, 0); }

int audiovideo_init() {
  //  disable Core 0 WDT
  TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
  esp_task_wdt_delete(idle_0);

  vidQueue = xQueueCreate(1, sizeof(uint8_t *));

  xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 0);
  debug("videoTask Pinned To Core 0...");
  return 0;
}

bool EXIT = false;

#define MAXFILES 512
char *filename[MAXFILES];
char fileext[4];
#define FILESPERPAGE 8

char *MAINPATH;
char textbuf[64] = {0};

// NES EMU SECTION
#include "NESemulator_part1.h"
#include "NESemulator_part2.h"
#include "mappers.h"

static void videoTask(void *arg) {
  while (1) {
    xQueueReceive(vidQueue, &screenMemory, portMAX_DELAY);
    nescreen::writeFrame(X_POS_OF_VIRTUAL_SCREEN, Y_POS_OF_VIRTUAL_SCREEN);
  }
}

//--------------------------------------------------------------------------------
// bool oscilloscope_installed = false;
// TaskHandle_t task_adc;
// void install_oscilloscope() {
// #if COMPOSITE_VIDEO_ENABLED
//   xTaskCreatePinnedToCore(core1_task, "adc_handle",
//                           20000,      // Stack size in words
//                           NULL,       // Task input parameter
//                           3,          // Priority of the task
//                           &task_adc,  // Task handle.
//                           1           // Core where the task should run
//   );
//   oscilloscope_installed = true;
// #endif
// }

void onKeysCallback(uint8_t keyMap) { debug("on key press: %d\n", keyMap); }
void onJoystickCallback(uint8_t joystickMoveMap) {
  debug("on joystic moved: %d\n", joystickMoveMap);
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

  // DAC COMPOSITE VIDEO & ADC has setup here:
#if COMPOSITE_VIDEO_ENABLED
  // start the A/V pump on app core
  initCompositeVideo(4, 2, nes_32bit, 1);
#endif
  audiovideo_init();

  I2S0.conf.rx_start = 0;  /// stop DMA ADC
  I2S0.in_link.start = 0;

  MAINPATH = (char *)malloc(256);

  fStorageInit();

  install_timer(60);  // 60Hz
  getMemoryStatus();

  menuInit();
  updateScreen();
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
    updateScreen();
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
