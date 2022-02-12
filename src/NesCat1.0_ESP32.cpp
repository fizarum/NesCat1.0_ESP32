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
//* *
//*   NCAT SYSTEM 1.0 by Nathalis *
//* *
//*   Includes: 1. NES EMULATOR, 2. MP3 player, 3. Oscilloscope 1MHz *
//* *
//*   Requirements: NodeMCU ESP32S, ILI9341 LCD, MICROSD CARD slot, *
//*   PCM5102 I2S AUDIO MODULE, OTHER PARTS... *
//* *
//*   Only for personal & educational use! *
//*   (GPL-3.0 License) *
//* *
//********************************************************************************
//* *
//*   features: *
//*   - ESP32-WROVER PSRAM module support *
//*   - MicroSD card support *
//*   - 320*240 2.8" LCD ILI9341 display *
//*   - Composite TV OUT Video PAL *
//*   - I2S AUDIO support PCM5102 module *
//*   - PS2 (USB) KEYBOARD support (wireless not work) *
//*   - huge NES ROMs up to 512kB (read from FLASH) *
//* *
//********************************************************************************

#define LCD_ENABLED true
#define COMPOSITE_VIDEO_ENABLED true  // Do not disable! it also disable ADC.
#define KEYBOARD_ENABLED true
#define SOUND_ENABLED true

#define DEBUG true        // Serial debugging enable.
#define DEBUGEXTRA false  // Extra Serial debugging enable.

//********************************************************************************

#include <controls.h>

///!!! do not forget 1KOHM resistors
#define KEYBOARD_DATA 4  /// ---[ 1K ]--- // -D
#define KEYBOARD_CLK 0   /// ---[ 1K ]--- // +D

// COMPOSITE_VIDEO: - //DAC_GPIO25_CHANNEL or DAC_GPIO26_CHANNEL
#define VIDEO_OUT (DAC_GPIO26_CHANNEL)

// AUDIO_i2S:
#define I2S_BCK_IO (GPIO_NUM_27)  // BCK
#define I2S_WS_IO (GPIO_NUM_32)   // LCK
#define I2S_DO_IO (GPIO_NUM_25)   // DIN
#define I2S_DI_IO (-1)

// display part
#include <display.h>

// SD card part
// todo: change after display testing
#define SOFTSD_MOSI_PIN TFT_MOSI
#define SOFTSD_MISO_PIN TFT_MISO
#define SOFTSD_SCK_PIN TFT_CLK
#define SD_CS_PIN 16

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
// #include <Audio.h>
#include <SdFat.h>
// #include <arduinoFFT.h>

// arduinoFFT FFT = arduinoFFT();
// SETUP:
#define SPI_CLOCK SD_SCK_MHZ(30)
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)

SdFat sd;
File fp;

static void videoTask(void *arg);

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes
                    // in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in
                    // FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in
                    // FLASH, 96 characters

//********************************************************************************
// VARIABLES:

uint32_t PSRAMSIZE = 0;
uint8_t *PSRAM;

//--------------------------------------------------------------------------------
// SPI FLASH MEMORY ACCESS:
#include "esp_spi_flash.h"

#define SPI_FLASH_ADDRESS 0x00300000  // 0x00300000 working (empty flash area)
#define SPI_FLASH_SECTOR_SIZE 0x1000  // 4kB = better not change

uint32_t FILE_ROM_SIZE = 0;
uint32_t FLASH_ROM_SIZE = 0;

// constant data pointer for direct access
const void *ROM;

spi_flash_mmap_handle_t handle1;

uint8_t flashdata[4096] = {0};  // 4kB buffer

//--------------------------------------------------------------------------------
// DRAM MEMORY ACCESS:
//--------------------------------------------------------------------------------

char *ROMFILENAME;       // NES load File name
unsigned char *rom = 0;  // Actual ROM pointer

//********************************************************************************

// for player...
bool PLAYING = false;
bool PAUSED = false;

//--------------------------------------------------------------------------------
uint8_t scancode = 0;
boolean keyup = false;
byte keymap[256];
//--------------------------------------------------------------------------------
IRAM_ATTR void USB_KEYBOARD() {
#if KEYBOARD_ENABLED
  if (keymap[0x75] == 0) {
    JOY_UP = 1;
  } else
    JOY_UP = 0;
  if (keymap[0x72] == 0) {
    JOY_DOWN = 1;
  } else
    JOY_DOWN = 0;
  if (keymap[0x6B] == 0) {
    JOY_LEFT = 1;
  } else
    JOY_LEFT = 0;
  if (keymap[0x74] == 0) {
    JOY_RIGHT = 1;
  } else
    JOY_RIGHT = 0;

  if (keymap[0x11] == 0 || keymap[0x1A] == 0)
    JOY_CROSS = 1;  // ALT or X
  else
    JOY_CROSS = 0;
  if (keymap[0x14] == 0 || keymap[0x22] == 0)
    JOY_SQUARE = 1;  // CTRL or Z
  else
    JOY_SQUARE = 0;
  if (keymap[0x5A] == 0)
    JOY_SHARE = 1;
  else
    JOY_SHARE = 0;
  if (keymap[0x66] == 0)
    JOY_OPTIONS = 1;
  else
    JOY_OPTIONS = 0;

  if (keymap[0x76] == 0) {
    JOY_SHARE = 1;
    JOY_OPTIONS = 1;
  } else if (JOY_SHARE == 1 && JOY_OPTIONS == 1) {
    JOY_SHARE = 0;
    JOY_OPTIONS = 0;
  }

  if (DEBUGEXTRA) {
    if (JOY_UP) Serial.print("UP.");
    if (JOY_DOWN) Serial.print("DOWN.");
    if (JOY_LEFT) Serial.print("LEFT.");
    if (JOY_RIGHT) Serial.print("RIGHT.");
    if (JOY_SHARE) Serial.print("START.");
    if (JOY_OPTIONS) Serial.print("SELECT.");
    if (JOY_CROSS) Serial.print("A.");
    if (JOY_SQUARE) Serial.print("B.");
    Serial.println();
  }
#endif
}
//--------------------------------------------------------------------------------
#if KEYBOARD_ENABLED
void IRAM_ATTR kb_interruptHandler(void) {
  static uint8_t bitcount = 0;
  uint8_t val;

  int clock = digitalRead(KEYBOARD_CLK);
  if (clock == 0) {
    return;
  }

  val = digitalRead(KEYBOARD_DATA);
  if (DEBUGEXTRA) Serial.print(".");
  if (DEBUGEXTRA) Serial.print(val);

  bitcount++;

  if (bitcount > 1 && bitcount < 10) {  // 8bits
    scancode |= ((val & 1) << (bitcount - 2));
  }

  if (bitcount > 10) {
    if (keyup == true) {
      keymap[scancode] = 1;

      if (DEBUGEXTRA) {
        Serial.print(" {");
        Serial.print(scancode, HEX);
        Serial.print("} ");
      }
      keyup = false;
    } else {
      keymap[scancode] = 0;
    }

    if (scancode == 0xF0) {
      keyup = true;
    } else {
      keyup = false;
    }

    if (DEBUGEXTRA) {
      Serial.print("[");
      Serial.print(scancode, HEX);
      Serial.println("]");
    }

    USB_KEYBOARD();

    bitcount = 0;
    scancode = 0;
  }
}
#endif
//--------------------------------------------------------------------------------
#if KEYBOARD_ENABLED
void kb_begin() {
  pinMode(KEYBOARD_DATA, INPUT_PULLUP);
  pinMode(KEYBOARD_CLK, INPUT_PULLUP);
  digitalWrite(KEYBOARD_DATA, true);
  digitalWrite(KEYBOARD_CLK, true);
  attachInterrupt(digitalPinToInterrupt(KEYBOARD_CLK), kb_interruptHandler,
                  RISING);
  memset(keymap, 1, sizeof(keymap));
}
#endif
//********************************************************************************

//===============================================================================
// INCLUDES:

// COMPOSITE_VIDEO_OUT
#if COMPOSITE_VIDEO_ENABLED
#include "compositevideo.h"
#endif

// MAIN FONT
#include "Retro8x16.c"  //used font...

//===============================================================================
//===============================================================================
//===============================================================================
// VIDEO SYSTEM:
QueueHandle_t vidQueue;
//--------------------------------------------------------------------------------
// BUFFER TEXT DRAW FUNCTIONS
//--------------------------------------------------------------------------------
inline void updateScreen() {
#ifdef LCD_ENABLED
  xQueueSend(vidQueue, &screenMemory, 0);
#endif
}
//--------------------------------------------------------------------------------
void screenmemory_fillscreen(uint8_t colorIndex = UNIVERSAL_BKG_COLOR) {
  nescreen::fillscreen(colorIndex);
  updateScreen();
}
//--------------------------------------------------------------------------------
void screenmemory_line(int startx, int starty, int endx, int endy,
                       uint8_t color) {
  int t, distance;
  int xerr = 0, yerr = 0, delta_x, delta_y;
  int incx, incy;
  // compute the distances in both directions
  delta_x = endx - startx;
  delta_y = endy - starty;
  // Compute the direction of the increment,
  //   an increment of 0 means either a horizontal or vertical
  //   line.
  if (delta_x > 0)
    incx = 1;
  else if (delta_x == 0)
    incx = 0;
  else
    incx = -1;

  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0;
  else
    incy = -1;

  // determine which distance is greater
  delta_x = abs(delta_x);
  delta_y = abs(delta_y);
  if (delta_x > delta_y)
    distance = delta_x;
  else
    distance = delta_y;

  // draw the line
  for (t = 0; t <= distance + 1; t++) {
    nescreen::drawPixel(startx, starty, color);

    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      startx += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      starty += incy;
    }
  }
}
//--------------------------------------------------------------------------------
void screenmemory_drawrectangle(int16_t X, int16_t Y, int16_t Width,
                                int16_t Height, uint8_t COLOR) {
  screenmemory_line(X, Y, X + Width, Y, COLOR);
  screenmemory_line(X, Y, X, Y + Height, COLOR);
  screenmemory_line(X + Width, Y, X + Width, Y + Height, COLOR);
  screenmemory_line(X, Y + Height, X + Width, Y + Height, COLOR);
}
//--------------------------------------------------------------------------------
void screenmemory_drawfillrectangle(int16_t X, int16_t Y, int16_t Width,
                                    int16_t Height, uint8_t COLOR) {
  for (uint16_t Ypos = Y; Ypos < Y + Height; Ypos++)
    for (uint16_t Xpos = X; Xpos < X + Width; Xpos++) {
      nescreen::drawPixel(Xpos, Ypos, COLOR);
    }
}
//--------------------------------------------------------------------------------
void set_font_XY(uint16_t x, uint16_t y) { nescreen::setTextPosition(x, y); }
//--------------------------------------------------------------------------------
void draw_string(const char *c, uint8_t color = 48) {
  if (c[strlen(c) - 1] == '\n') {
    nescreen::drawString(c, color);
    nescreen::setTextNewLine();
  } else {
    nescreen::drawString(c, color);
  }
}
//--------------------------------------------------------------------------------

int audiovideo_init() {
  // rendererInit(&videoTask);
  //  disable Core 0 WDT
  TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
  esp_task_wdt_delete(idle_0);

  // todo: recheck various values of queue length
  vidQueue = xQueueCreate(64, sizeof(uint8_t *));

#ifdef LCD_ENABLED
  xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 0, NULL, 0);
  debug("videoTask Pinned To Core 0...");
#endif  // LCD_ENABLED
  return 0;
}
//--------------------------------------------------------------------------------

//********************************************************************************
//*  MAIN MENU: *
//********************************************************************************
uint8_t MenuItem = 0;
uint8_t prevMenuIndex = 100;

// 0 menu index
const char *const emulator = " NES Emulator ";
// 1 menu index
const char *const audioPlayer = " Audio Player ";
// 2 menu index
const char *const oscilloscope = " Oscilloscope ";

const uint8_t yPosOfMenuItem = V_CENTER - 24;

void displayText(const char *text, uint8_t x, uint8_t y) {
  nescreen::drawString(x, y, text, 48);
}

/**
 * @brief used to draw menu item, like "Nes emulator", etc.
 * @param x horizontal position of menu item, may vary depends on slide
 * animation
 *
 */
void displayMenuItem(const char *text, uint8_t x) {
  displayText(text, x, yPosOfMenuItem);
}

uint8_t updateActiveMenuIndex() {
  if (isJoystickMoved() == 0) return MenuItem;

  //--------------------------------------------------------------------------------
  if (JOY_RIGHT == 1) {
    debug("moved joy right");
    JOY_RIGHT = 0;
    MenuItem++;
    if (MenuItem > 2) MenuItem = 0;
  }

  if (JOY_LEFT == 1) {
    debug("moved joy left");
    JOY_LEFT = 0;
    MenuItem--;
    // since we have uint8_t it cant be < 0 but uint8_t.MAX (255)
    if (MenuItem > 2) MenuItem = 2;
  }

  if ((JOY_SQUARE == 1 || JOY_SHARE == 1) && JOY_OPTIONS == 0) {
    JOY_SQUARE = 0;
    JOY_SHARE = 0;
    MenuItem++;
    if (MenuItem > 2) MenuItem = 0;
  }
  return MenuItem;
}
//********************************************************************************

bool EXIT = false;

#define MAXFILES 512
char *filename[MAXFILES];
char fileext[4];
#define FILESPERPAGE 8

SdFile dirFile;
SdFile file;

char *MAINPATH;
char textbuf[64] = {0};

//--------------------------------------------------------------------------------
void secondsToHMS(const uint32_t seconds, uint16_t &h, uint8_t &m, uint8_t &s) {
  uint32_t t = seconds;
  s = t % 60;
  t = (t - s) / 60;
  m = t % 60;
  t = (t - m) / 60;
  h = t;
}
//--------------------------------------------------------------------------------

//________________________________________________________________________________
//
// NES EMU SECTION
//________________________________________________________________________________

#include "NESemulator_part1.h"

// MAPPERS !!!
#include "NESemulator_part2.h"
#include "mappers.h"

//________________________________________________________________________________
//
// PLAYER SECTION
//________________________________________________________________________________
// Audio audio;

#include "player.h"

//________________________________________________________________________________
//
// OSCILLOSCOPE SECTION
//________________________________________________________________________________

//--------------------------------------------------------------------------------
// OSC MAIN DEFINITIONS:
#define NUM_SAMPLES 1000  // number of samples
#define I2S_NUM (0)
#define BUFF_SIZE 50000
#define B_MULT (BUFF_SIZE / NUM_SAMPLES)

#include "oscilloscope.h"

// //--------------------------------------------------------------------------------
static void videoTask(void *arg) {
#ifdef LCD_ENABLED
  while (1) {
    xQueueReceive(vidQueue, &screenMemory, portMAX_DELAY);
    nescreen::writeFrame(X_POS_OF_VIRTUAL_SCREEN, Y_POS_OF_VIRTUAL_SCREEN);
    if (PLAYING) visualyze();  // visualyze when playing...
  }
#endif
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
//********************************************************************************
rominfo_t *rominfo;
unsigned char *romdata = 0;
char *PATH;
//================================================================================
char loadmessage[64];

unsigned char *getromdata(char *ROMFILENAME_) {
  fp = sd.open(ROMFILENAME_);
  if (DEBUG) Serial.print("FILE SIZE: ");
  if (DEBUG) Serial.println(fp.size());
  FILE_ROM_SIZE = fp.size();

  uint16_t BLOCKCOUNT =
      (FILE_ROM_SIZE + SPI_FLASH_SECTOR_SIZE) / SPI_FLASH_SECTOR_SIZE;
  uint16_t BLOCKSIZEPX = DEFAULT_WIDTH / BLOCKCOUNT;
  Serial.print("BLOCKCOUNT: ");
  Serial.print(BLOCKCOUNT);
  Serial.print(" BLOCKSIZEPX: ");
  Serial.print(BLOCKSIZEPX);
  Serial.println();
  BLOCKSIZEPX++;

  if (PSRAMSIZE > 0) {
    uint32_t i = 0;
    for (i = 0; i < fp.size(); i++) {
      PSRAM[i] = fp.read();
    }
    fp.close();
    return (unsigned char *)PSRAM;
  } else {
    //********************************************************************************
    // Read NES rom to SPI FLASH!
    uint32_t i = 0;
    for (i = 0; i < fp.size() + SPI_FLASH_SECTOR_SIZE; i++) {
      if (DEBUGEXTRA && i % SPI_FLASH_SECTOR_SIZE == 0) {
        Serial.print("ROM loaded 4kB:");
        Serial.println(i / 0x1000);
      }
      if (i > 0 && i % SPI_FLASH_SECTOR_SIZE == 0) {
        /// spi_flash_erase_sector(SPI_FLASH_ADDRESS/SPI_FLASH_SECTOR_SIZE +
        /// (i/SPI_FLASH_SECTOR_SIZE)-SPI_FLASH_SECTOR_SIZE);
        delayMicroseconds(300);
        spi_flash_erase_range(SPI_FLASH_ADDRESS + i - SPI_FLASH_SECTOR_SIZE,
                              sizeof(flashdata));
        delayMicroseconds(300);
        spi_flash_write(SPI_FLASH_ADDRESS + i - SPI_FLASH_SECTOR_SIZE,
                        flashdata, sizeof(flashdata));
        delayMicroseconds(300);

        sprintf(loadmessage, " %d / %d", i, FILE_ROM_SIZE);
        set_font_XY(8, 8);
        draw_string("Loaded:");
        draw_string(loadmessage);
        screenmemory_drawfillrectangle(
            ((i / SPI_FLASH_SECTOR_SIZE) - 1) * BLOCKSIZEPX, 24, BLOCKSIZEPX,
            16, 57);
      }
      delayMicroseconds(50);
      if (fp.available()) flashdata[i % SPI_FLASH_SECTOR_SIZE] = fp.read();
      delayMicroseconds(50);
    }
    fp.close();

    FLASH_ROM_SIZE = i;  // Size of File and Offset Align

    debug("FLASH SIZE: %d", FLASH_ROM_SIZE);

    ROM = 0;
    /// if (handle1) spi_flash_munmap(handle1);
    printf("Mapping %x (+%x)\n", SPI_FLASH_ADDRESS, FLASH_ROM_SIZE);
    ESP_ERROR_CHECK(spi_flash_mmap(SPI_FLASH_ADDRESS, FLASH_ROM_SIZE,
                                   SPI_FLASH_MMAP_DATA, &ROM, &handle1));
    printf("mmap_res: handle=%d ptr=%p\n", handle1, ROM);
    debug("[NES ROM MAPPED ON FLASH!]");
    return (unsigned char *)ROM;
  }
}
//================================================================================

//********************************************************************************
//*  SETUP: *
//********************************************************************************
void setup() {
  Serial.begin(115200);
  getMemoryStatus();

  if (ESP.getPsramSize() > 0) {
    PSRAMSIZE = ESP.getPsramSize();
    PSRAM = (uint8_t *)ps_malloc(2097152);  // PSRAM malloc 2MB

    debug("Total PSRAM: %d", ESP.getPsramSize());
    debug("Free PSRAM: %d", ESP.getFreePsram());
  } else {
    PSRAMSIZE = 0;
    debug("NO PSRAM DETECTED.");
  }

  //--------------------------------------------------------------------------------
  // VIDEO MEMORY ALLOCATION (force)
  for (uint8_t tmp = 0; tmp < 240; tmp++) {
    screenMemory[tmp] = (uint8_t *)malloc(256 + 1);
    memset(screenMemory[tmp], 0, 256);
  }
  //--------------------------------------------------------------------------------
  // Buttons Pins Input Init
  pinMode(PIN_A, INPUT);               // A
  pinMode(PIN_B, INPUT);               // B
  pinMode(PIN_SELECT, INPUT);          // SELECT
  pinMode(PIN_START, INPUT);           // START
  pinMode(PIN_UP, INPUT);              // UP
  pinMode(PIN_DOWN, INPUT);            // DOWN  //TCK
  pinMode(PIN_LEFT, INPUT);            // LEFT
  pinMode(PIN_RIGHT, INPUT_PULLDOWN);  // RIGHT
  //--------------------------------------------------------------------------------
#if LCD_ENABLED
  displayInit();
#endif
  //--------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------
  // DAC COMPOSITE VIDEO & ADC has setup here:
#if COMPOSITE_VIDEO_ENABLED
  initCompositeVideo(4, 2, nes_32bit, 1);  // start the A/V pump on app core
#endif
  //--------------------------------------------------------------------------------
  // SCREENMEMORY LCD DRAW INIT
  audiovideo_init();
  //--------------------------------------------------------------------------------
  // PS2/USB KEYBOARD SUPPORT
#if KEYBOARD_ENABLED
  kb_begin();
#endif
  //--------------------------------------------------------------------------------
  nescreen::setFont(Retro8x16);
  //--------------------------------------------------------------------------------
  I2S0.conf.rx_start = 0;  /// stop DMA ADC
  I2S0.in_link.start = 0;
  //--------------------------------------------------------------------------------
  // malloc MAINPATH for player browser
  MAINPATH = (char *)malloc(256);
  //--------------------------------------------------------------------------------
  // microSD CARD INIT
  if (!sd.begin(SD_CONFIG)) {
    debug("SD error!");
  } else {
    debug("SD OK.");
  }
  //--------------------------------------------------------------------------------
  install_timer(60);  // 60Hz
  //--------------------------------------------------------------------------------
  ///... SETUP DONE

  getMemoryStatus();

  debug("setup finished");
}

//********************************************************************************
//*  LOOP: *
//********************************************************************************

int32_t tickcnt = 0;

void drawActiveMenu() {
  switch (MenuItem) {
    case 0:
      displayMenuItem(emulator, 64);
      break;
    case 1:
      displayMenuItem(audioPlayer, 64);
      break;

    case 2:
      displayMenuItem(oscilloscope, 64);
      break;

    default:
      break;
  }
}

void loop() {
  EXIT = false;
  MenuItem = updateActiveMenuIndex();
  if (MenuItem != prevMenuIndex) {
    screenmemory_fillscreen(0x0c);
    debug("menu item: %d", MenuItem);
    drawActiveMenu();
    updateScreen();
    prevMenuIndex = MenuItem;
  }

  switch (MenuItem) {
    case 1:
      // nes emulator part. code has been removed to save your mental health
      break;
    case 2:
      // audio player part. code has been removed to save your mental health
      break;
    case 3:
      // oscilloscope part. code has been removed to save your mental health
      break;
    default:
      break;
  }
  delay(100);
  JOY_SHARE = 0;
  JOY_OPTIONS = 0;
  EXIT = false;
}
