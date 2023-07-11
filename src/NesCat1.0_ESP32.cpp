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
#include <app_menu.h>
#include <audio.h>
#include <controls.h>
#include <display.h>
#include <utils.h>

// start app - menu
Menu menu;

void onAppClosedCallback();
void onInputTriggered(uint16_t keyMap);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial completion init
  }
  controlsInit(&onInputTriggered);

  displayInit();

  //initVideo();

  // install_timer(60);  // 60Hz
  // getMemoryStatus();

  menu.open(&onAppClosedCallback);
}

void loop() {
  controlsUpdate();
  menu.update();
}

void onAppClosedCallback() { menu.closeUserApp(); }

void onInputTriggered(uint16_t keyMap) {
  if (menu.handle(keyMap) == false) {
    // print warning if its required
  }
}