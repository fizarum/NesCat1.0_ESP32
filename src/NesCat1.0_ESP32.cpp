//                          ▄▀▀█▄                ▄██▀▀▄       
//                         ▐█░░ ▀██▄          ▄███▀░░░█       
//                         ▐█░░░░░▀█████████████▀▒░░░░█       
//                 ▄▄████████░░▒▒▒▒░██████████████░▒░░█       
//             ▄█████████████▄▒▒▒▒▒▄████████████████░▐        
//           ▄████████████████▄▄▄█████████  ▀████████▌        
//          ▐█████████▀▀▀▀▀█████████████▀    ▀███████▌        
//         ▐█▀███▀▀▄       ▐██████████▀        ▀█████▌        
//         ▓▌               ░████▀▀▀   ▀▄    ▄▀   ▀██         
//        ▄██              ▐ ▀▌                    ▄▀         
//       ▓███▒             ▐  ▀▓▄▒         ▄    ▒▄▀▀          
//       █████▒▒         ▒▒▌             ▀▀▒▀▀▀ ▀█            
//       ▓████████████████████▒▒         ▄ ▄▄ ▒▄▒▄█▄▄         
//     ▄▄███████████████████████████████████▀██████████▄▄     
//  ▄█▀▒▒▀▒▒▒▀█▓▀▒█▓▒▒▓█▀▒▒▀█▄▒▒▒▒█▄████▓▓▓███▒███▓▓████▓██   
// ▐█▒▒▒▒▒▒▒▀▒▄▒▀▒▄▄▒▒▀█▄▒▒▒▄█▒▓▄█████▓▓███████▓▓▒▒███▀▒▓▒▒██▄
//  ▀▀█▄▒▒▒▒▄▄▒▒▒▄▒▒█▄▒▒▒▒▀▒▒▒▒▒▄███▄██▀▒██▒██▒▓▓▓▓▓▓▓▓▒▄█▀▀  
//      ▀▀█▄▒▒██▒▒█████████▒▒▒▒██▀▄██▒▒▄▓▓▓▓▓▒▒▓▓▓▓▒▄█▀▀      
//          ▀▀████▓████▓████▒▒▒▒▒▒▒▄▒▀▄█▓██▒▒▓▓▒▄█▀▀          
//              ▀▀███████▀▀▀▒▒▄▄▀▀▒▄▓█▓▒▒▓█▒██▀▀              
//                  ▀███▀████▄▒▓▓█▌▒▒▓█▒██▀▀                  
//                      ▀██▒▒▒▒▓▓▓▓▒███▀                      
//                          ▀▀▀▀▀▀▀▀                          

//********************************************************************************
//*
//*   NCAT SYSTEM 1.0 by Nathalis
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
//*   - PS2 (USB) KEYBOARD support (wireless not work)
//*   - huge NES ROMs up to 512kB (read from FLASH)
//*
//********************************************************************************

#define COMPOSITE_VIDEO_ENABLED true  // Do not disable! it also disable ADC.
#define SOUND_ENABLED true

#define DEBUG true        // Serial debugging enable.
#define DEBUGEXTRA false  // Extra Serial debugging enable.

//********************************************************************************

#include <controls.h>

// COMPOSITE_VIDEO: - //DAC_GPIO25_CHANNEL or DAC_GPIO26_CHANNEL
#define VIDEO_OUT (DAC_GPIO26_CHANNEL)

// AUDIO_i2S:
#define I2S_BCK_IO (GPIO_NUM_27)  // BCK
#define I2S_WS_IO (GPIO_NUM_32)   // LCK
#define I2S_DO_IO (GPIO_NUM_25)   // DIN
#define I2S_DI_IO (-1)

// display part
#include <display.h>
#include <menu.h>

// SD card part
#define SOFTSD_MOSI_PIN TFT_MOSI
#define SOFTSD_MISO_PIN TFT_MISO
#define SOFTSD_SCK_PIN TFT_CLK
#define SD_CS_PIN 34

// Oscilloscope INPUT:
#define ADC_CHANNEL ADC1_CHANNEL_6  // GPIO34

// player Digital I/O used
//#define SD_CS          -1
#define SPI_MOSI SOFTSD_MOSI_PIN
#define SPI_MISO SOFTSD_MISO_PIN
#define SPI_SCK SOFTSD_SCK_PIN
#define I2S_DOUT I2S_DO_IO
#define I2S_BCLK I2S_BCK_IO
#define I2S_LRC I2S_WS_IO
//********************************************************************************
// MAIN LIBRARIES:

#include <esp_task_wdt.h>

#if SOUND_ENABLED
#include "driver/i2s.h"
#endif  // SOUND_ENABLED
#include "driver/dac.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_intr_alloc.h"
#include "esp_types.h"
#include "rom/gpio.h"
#include "rom/lldesc.h"
#include "soc/gpio_reg.h"
#include "soc/i2s_reg.h"
#include "soc/i2s_struct.h"
#include "soc/io_mux_reg.h"
#include "soc/ledc_struct.h"
#include "soc/rtc.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc.h"
#include "utils.h"

// LIBRARIES:
#include <Arduino.h>

// micro_SD_Card
#include <SdFat.h>

// #include <Audio.h>
// #include <arduinoFFT.h>
// arduinoFFT FFT = arduinoFFT();
#define SPI_SPEED_FOR_SD SD_SCK_MHZ(4)

SdFat sd;

static void videoTask(void *arg);

//********************************************************************************
// VARIABLES:

uint32_t psRAMSize = 0;
uint8_t *PSRAM;

//--------------------------------------------------------------------------------
// SPI FLASH MEMORY ACCESS:
#include "esp_spi_flash.h"

//--------------------------------------------------------------------------------
// DRAM MEMORY ACCESS:
//--------------------------------------------------------------------------------

char *ROMFILENAME;       // NES load File name
unsigned char *rom = 0;  // Actual ROM pointer

// COMPOSITE_VIDEO_OUT
#if COMPOSITE_VIDEO_ENABLED
#include "compositevideo.h"
#endif

// MAIN FONT
#include "Retro8x16.c"  //used font...

//===============================================================================
// VIDEO SYSTEM:
QueueHandle_t vidQueue;

inline void updateScreen() { xQueueSend(vidQueue, &screenMemory, 0); }

void screenmemory_fillscreen(uint8_t colorIndex = UNIVERSAL_BKG_COLOR) {
  nescreen::fillscreen(colorIndex);
  updateScreen();
}

void set_font_XY(uint16_t x, uint16_t y) { nescreen::setTextPosition(x, y); }

void draw_string(const char *c, uint8_t color = MENU_TEXT_COLOR,
                 uint8_t bkgColor = MENU_BACKGROUND_COLOR) {
  if (c[strlen(c) - 1] == '\n') {
    nescreen::drawString(c, color, bkgColor);
    nescreen::setTextNewLine();
  } else {
    nescreen::drawString(c, color, bkgColor);
  }
}
//--------------------------------------------------------------------------------

int audiovideo_init() {
  //  disable Core 0 WDT
  TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
  esp_task_wdt_delete(idle_0);

  vidQueue = xQueueCreate(1, sizeof(uint8_t *));

  xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 0);
  debug("videoTask Pinned To Core 0...");
  return 0;
}

//********************************************************************************
//*  MAIN MENU: *
//********************************************************************************
const uint8_t yPosOfMenuItem = V_CENTER - 24;

/**
 * @brief used to draw menu item, like "Nes emulator", etc.
 * @param x horizontal position of menu item, may vary depends on slide
 * animation
 *
 */
void displayMenuItem(const char *text, uint8_t x) {
  nescreen::drawString(x, yPosOfMenuItem, text, MENU_TEXT_COLOR,
                       MENU_BACKGROUND_COLOR);
}

bool EXIT = false;

#define MAXFILES 512
char *filename[MAXFILES];
char fileext[4];
#define FILESPERPAGE 8

SdFile dirFile;
SdFile file;

char *MAINPATH;
char textbuf[64] = {0};

// NES EMU SECTION
#include "NESemulator_part1.h"
#include "NESemulator_part2.h"
#include "mappers.h"
#include "oscilloscope.h"

static void videoTask(void *arg) {
  while (1) {
    xQueueReceive(vidQueue, &screenMemory, portMAX_DELAY);
    nescreen::writeFrame(X_POS_OF_VIRTUAL_SCREEN, Y_POS_OF_VIRTUAL_SCREEN);
  }
}

//--------------------------------------------------------------------------------
bool oscilloscope_installed = false;
TaskHandle_t task_adc;
void install_oscilloscope() {
#if COMPOSITE_VIDEO_ENABLED
  xTaskCreatePinnedToCore(core1_task, "adc_handle",
                          20000,      // Stack size in words
                          NULL,       // Task input parameter
                          3,          // Priority of the task
                          &task_adc,  // Task handle.
                          1           // Core where the task should run
  );
  oscilloscope_installed = true;
#endif
}

void onKeysCallback(uint8_t keyMap) { debug("on key press: %d\n", keyMap); }

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

  controlsInit(&onKeysCallback);

  displayInit();
  nescreen::setFont(Retro8x16);

  // DAC COMPOSITE VIDEO & ADC has setup here:
#if COMPOSITE_VIDEO_ENABLED
  // start the A/V pump on app core
  initCompositeVideo(4, 2, nes_32bit, 1);
#endif
  // SCREENMEMORY LCD DRAW INIT
  audiovideo_init();

  I2S0.conf.rx_start = 0;  /// stop DMA ADC
  I2S0.in_link.start = 0;

  // malloc MAINPATH for player browser
  MAINPATH = (char *)malloc(256);

  // microSD CARD INIT
  if (!sd.begin(SD_CS_PIN, SPI_SPEED_FOR_SD)) {
    debug("SD error!, code: %d", sd.card()->errorCode());
  } else {
    debug("SD OK.");
  }

  install_timer(60);  // 60Hz
  getMemoryStatus();
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
  if (hJoyPos != 0) {
    const char *title = getCurrentMenuTitle();
    nescreen::fillscreen(0x0c);
    displayMenuItem(title, 64);
    updateScreen();
    debug("menu item: %s", title);
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
