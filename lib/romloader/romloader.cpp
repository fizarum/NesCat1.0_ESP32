#include "romloader.h"

#include <SdFat.h>
#include <esp32-hal.h>
#include <esp_err.h>
#include <esp_spi_flash.h>
#include <utils.h>

#include "NESemulator_part1.h"

#define SPI_FLASH_ADDRESS 0x00300000  // 0x00300000 working (empty flash area)
#define SPI_FLASH_SECTOR_SIZE 0x1000  // 4kB = better not change

uint32_t FLASH_ROM_SIZE = 0;

spi_flash_mmap_handle_t handle1;

rominfo_t *rominfo;
unsigned char *romdata = 0;

char loadmessage[64];

char *ROMFILENAME;  // NES load File name

// constant data pointer for direct access
const void *ROM;

uint8_t flashdata[4096] = {0};  // 4kB buffer

FsFile fp;

// void openFile(SdFat *sd, const char *filename) { fp = sd->open(filename); }
// void printFileSize() { debug("file size: %u (Kb)", fp.size() / 1024); }

// void loadToPsRam(uint8_t *psRam) {
//   printFileSize();
//   for (uint32_t i = 0; i < fp.size(); i++) {
//     psRam[i] = fp.read();
//   }
//   fp.close();
// }

unsigned char *getromdata(SdFat *sd, const char *filename) {
  fp = sd->open(filename);

  const uint64_t fileSize = fp.size();

  debug("file size: %u (Kb)", fileSize / 1024);

  uint16_t blockCount =
      (fileSize + SPI_FLASH_SECTOR_SIZE) / SPI_FLASH_SECTOR_SIZE;
  Serial.print("block count: ");
  Serial.print(blockCount);

  if (psRAMSize > 0) {
    uint32_t i = 0;
    for (i = 0; i < fp.size(); i++) {
      PSRAM[i] = fp.read();
    }
    fp.close();
    return (unsigned char *)PSRAM;
  } else {
    // Read NES rom to SPI FLASH!
    uint32_t i = 0;
    uint16_t writtenBytesOfNewSector = 0;
    for (i = 0; i < fp.size() + SPI_FLASH_SECTOR_SIZE; i++) {
      writtenBytesOfNewSector = i % SPI_FLASH_SECTOR_SIZE;
      if (writtenBytesOfNewSector == 0) {
        debug("ROM loaded 4kB: %u", i / SPI_FLASH_SECTOR_SIZE);
      }
      if (i > 0 && writtenBytesOfNewSector == 0) {
        spi_flash_erase_range(SPI_FLASH_ADDRESS + i - SPI_FLASH_SECTOR_SIZE,
                              sizeof(flashdata));
        delayMicroseconds(300);
        spi_flash_write(SPI_FLASH_ADDRESS + i - SPI_FLASH_SECTOR_SIZE,
                        flashdata, sizeof(flashdata));
        delayMicroseconds(300);

        sprintf(loadmessage, " %d / %d", i, fileSize);
      }
      if (fp.available()) flashdata[writtenBytesOfNewSector] = fp.read();
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