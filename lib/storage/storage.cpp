#include "storage.h"

#include <SdFat.h>
#include <utils.h>

// SD module uses VSPI pins on NodeMCU ESP32S, same as for display
#define SD_CS_PIN 34
#define SD_FAT_TYPE 1

SdFat sd;
File32 tempFile;
File32 home;

const uint8_t maxLen = 100;
const uint8_t fname_max_len = 64;
const uint8_t smallBufferSize = 10;
char* fname = new char[fname_max_len];
// temp buffers
char* _temp_str = new char[maxLen];
char* _small_buffer = new char[smallBufferSize];

// todo: recheck what's the problem with:
// "[esp32-hal-gpio.c:130] __pinMode(): GPIO config failed"
void storageInit() {
  if (!sd.begin(SD_CS_PIN, SPI_DIV3_SPEED)) {
    debug("SD error!, code: %d", sd.card()->errorCode());
  } else {
    debug("SD OK.");
  }
}

void getFilenames(const char* path, void (*callback)(const char*),
                  const char* extension) {
  if (!home.open(path)) {
    debug("error opening: %s", path);
    return;
  }

  while (tempFile.openNext(&home, O_RDONLY)) {
    if (!tempFile.isHidden()) {
      char* filename = new char[fname_max_len];
      tempFile.getName(filename, fname_max_len);
      if (extension != nullptr) {
        if (hasSuchExtension(filename, extension)) {
          callback(filename);
        }
      } else {
        callback(filename);
      }
    }
    tempFile.close();
  }
  home.close();
}

bool hasSuchExtension(const char* filename, const char* extension) {
  if (filename == nullptr || extension == nullptr) {
    return false;
  }

  if (strlen(extension) >= smallBufferSize) {
    return false;
  }

  char* substring = strrchr(filename, '.');
  if (substring == nullptr) {
    return false;
  }
  //+1 to copy only extension
  strcpy(_temp_str, substring + 1);

  // make a lover case of it
  _temp_str = strlwr(_temp_str);

  // copy extension to prevent its modification as well
  strcpy(_small_buffer, extension);
  // and lover case for it too
  _small_buffer = strlwr(_small_buffer);

  return strstr(_temp_str, _small_buffer) != nullptr;
}