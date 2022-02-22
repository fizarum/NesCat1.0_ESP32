#include "fstorage.h"

#include <utils.h>

// SD module uses VSPI pins on NodeMCU ESP32S, same as for display
#define SD_CS_PIN 34

#define SPI_SPEED_FOR_SD SD_SCK_MHZ(4)

SdFat sd;

void fStorageInit() {
  if (!sd.begin(SD_CS_PIN, SPI_SPEED_FOR_SD)) {
    debug("SD error!, code: %d", sd.card()->errorCode());
  } else {
    debug("SD OK.");
  }
}