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
#include <app_container.h>
#include <configurator.h>
#include <device_manager.h>
#include <display/display_device.h>
#include <storage/storage_device.h>

#include "devices/audio/audio_device.h"
#include "devices/joystick/joystick_device.h"

DeviceManager deviceManager;
AppContainer appContainer;

DisplayDevice *displayDevice = nullptr;
JoystickDevice *joystick = nullptr;

void onInputTriggered();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial completion init
  }
  deviceManager.init();

#ifdef DISPLAY_ON
  displayDevice = new DisplayDevice();
  deviceManager.add(DEVICE_DISPLAY_ID, displayDevice);
#endif

#ifdef JOYSTICK_ON
  joystick = new JoystickDevice();
  joystick->setCallback(&onInputTriggered);
  deviceManager.add(DEVICE_JOYSTICK_ID, joystick);
#endif

#ifdef AUDIO_ON
  deviceManager.add(DEVICE_AUDIO_ID, new AudioDevice());
#endif

#ifdef STORAGE_ON
  deviceManager.add(DEVICE_STORAGE_ID, new StorageDevice());
#endif

  appContainer.init();
  appContainer.start();
}

void loop() {
  deviceManager.update();

  appContainer.update();
  appContainer.draw(displayDevice);
}

void onInputTriggered() { appContainer.handleInput(joystick); }