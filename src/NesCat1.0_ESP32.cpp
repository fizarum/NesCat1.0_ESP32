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
#include <device_manager.h>
#include <utils.h>

#include "../device/controls/joystick_device.h"

// start app - menu
Menu menu;

DeviceManager dm;

DisplayDevice *displayDevice = nullptr;
JoystickDevice *joystick = nullptr;

void onAppClosedCallback();
void onInputTriggered();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial completion init
  }

  dm.init();
  displayDevice = (DisplayDevice *)dm.get(DISPLAY_DEVICE);
  joystick = (JoystickDevice *)dm.get(JOYSTICK_DEVICE);
  joystick->setCallback(&onInputTriggered);

  menu.open(&onAppClosedCallback);
}

void loop() {
  dm.update();

  menu.update();
  menu.draw(displayDevice);
}

void onAppClosedCallback() { menu.closeUserApp(); }

void onInputTriggered() {
  if (menu.handle(joystick) == false) {
    // print warning if its required
  }
}