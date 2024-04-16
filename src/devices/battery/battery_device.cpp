#include "battery_device.h"

#include <esp32-hal-adc.h>
#include <esp32-hal-gpio.h>

#define BAT_LEVEL_PIN 34

const char* const BatteryDevice::getName() { return "battery"; }

bool BatteryDevice::onInit() {
  pinMode(BAT_LEVEL_PIN, INPUT_PULLUP);
  return true;
}

const uint32_t delayBetweenUpdates = 1000000;

void BatteryDevice::onUpdate() {
  now = esp_timer_get_time();

  if (lastUpdatedTimeAt + delayBetweenUpdates < now) {
    batteryLevel = analogReadMilliVolts(BAT_LEVEL_PIN);
    lastUpdatedTimeAt = now;
  }
}

uint32_t BatteryDevice::getBatteryVoltage() { return this->batteryLevel; }

uint8_t BatteryDevice::getBatteryLevel() {
  return (uint8_t)((float)this->batteryLevel / (float)BATTERY_LEVEL_MAX * 100);
}