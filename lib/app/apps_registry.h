#ifndef APPS_REGISTRY_H
#define APPS_REGISTRY_H

#include <stdint.h>

const uint8_t menuId = 0;

const uint8_t fileManagerId = 100;
const uint8_t nesEmulatorId = 101;
const uint8_t audioPlayerId = 102;
const uint8_t userSettingsId = 103;

const char *const menuTitle = "Menu";

const char *const fileManagerTitle = "File Manager";
const char *const emulatorTitle = "NES Emulator";
const char *const audioPlayerTitle = "Audio Player";
const char *const userSettingsTitle = "Settings";

#endif  // APPS_REGISTRY_H