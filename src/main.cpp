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
//*   - I2S AUDIO support PCM5102 module
//*   - huge NES ROMs up to 512kB (read from FLASH)
//*
//********************************************************************************
#include <Arduino.h>
#include <app_launcher.h>
#include <configurator.h>
#include <device_manager.h>
#include <scheduler.h>

// apps
#include "apps/demo/demo_app.h"
#include "apps/fm/app_file_manager.h"
#include "apps/nes_launcher/nes_launcher.h"
#include "apps/player/app_audio_player.h"
#include "apps/settings/app_settings.h"

// devices
#include "devices/audio/audio_device.h"
#include "devices/battery/battery_device.h"
#include "devices/display/ili9341_display.h"
#include "devices/joystick/joystick_device.h"
#include "devices/storage/storage_device.h"

DeviceManager deviceManager;
AppLauncher appLauncher;

ILI9341Display *display = nullptr;
JoystickDevice *joystick = nullptr;
BatteryDevice *battery = nullptr;

void onInputTriggered();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // wait for serial completion init
  }
  deviceManager.init();

  battery = new BatteryDevice();
  deviceManager.add(DEVICE_BATTERY_ID, battery);

#ifdef DISPLAY_ON
  display = new ILI9341Display();
  disableWDTForCore0();
  deviceManager.add(DEVICE_DISPLAY_ID, display);
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

  appLauncher.init();

  // add user apps
  appLauncher.add(new FileManager(), "File Manager");
  appLauncher.add(new Settings(), "Settings");
  appLauncher.add(new AudioPlayer(), "Audio Player");
  appLauncher.add(new NesLauncher(), "NES Emulator");
  appLauncher.add(new DemoApp(), "Demo App");

  appLauncher.start();
}

void loop() {
  deviceManager.update();

  appLauncher.update();
  appLauncher.draw(display);
}

void onInputTriggered() { appLauncher.handleInput(joystick); }