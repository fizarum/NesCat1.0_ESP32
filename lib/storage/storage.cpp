#include "storage.h"

#include <utils.h>

// SD module uses VSPI pins on NodeMCU ESP32S, same as for display
#define SD_CS_PIN 34
#define SD_FAT_TYPE 1

SdFat sd;
File32 tempFile;
File32 home;

uint8_t fname_max = 30;
char *fname = new char[fname_max];
// todo: recheck what's the problem with:
// "[esp32-hal-gpio.c:130] __pinMode(): GPIO config failed"
void storageInit() {
  if (!sd.begin(SD_CS_PIN, SPI_DIV3_SPEED)) {
    debug("SD error!, code: %d", sd.card()->errorCode());
  } else {
    debug("SD OK.");
  }
}

void printHomeFolder() {
  if (!home.open("/nes")) {
    debug("error opening /nes");
  }
  debug("/nes folder contains:\n");
  while (tempFile.openNext(&home, O_RDONLY)) {
    if (!tempFile.isHidden()) {
      tempFile.getName(fname, fname_max);

      debug("\tfile: %s", fname);
    }
    tempFile.close();
  }
}