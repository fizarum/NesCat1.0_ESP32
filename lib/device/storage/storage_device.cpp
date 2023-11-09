#include "storage_device.h"

#include <SD.h>
#include <SPI.h>
#include <esp_spi_flash.h>
#include <log.h>
#include <utils.h>

// SD module uses HSPI pins, except GPIO12(HSPI_MISO) - its used
// by system flash so its remapped to GPIO32
#define HSPI_MISO 32
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_CS 15

#define SD_FAT_TYPE 1

// 0x00300000 working (empty flash area)
#define SPI_FLASH_ADDRESS 0x00300000

#define BYTES_IN_MBYTE 1048576

SPIClass hspi = SPIClass(HSPI);

File home;
File tempFile;
bool isReading = false;

// 4kB = better not change
const uint16_t hugeBufferSize = 4096;
const uint8_t middleBufferSize = 64;
const uint8_t smallBufferSize = 10;

char* fname = new char[middleBufferSize];
// temp buffers
char* _temp_str = new char[middleBufferSize];
char* _small_buffer = new char[smallBufferSize];

// used for file reading
uint8_t _file_buff[hugeBufferSize];

// used for spi_flash_mmap
spi_flash_mmap_handle_t spiFlashMmapHandle;

bool StorageDevice::onInit() {
  hspi.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
  return SD.begin(HSPI_CS, hspi);
}

void StorageDevice::onUpdate() {
  // do nothing
}

void StorageDevice::onEnabled(bool enabled) {
  // do nothing as well
}

uint32_t StorageDevice::totalMBytes() {
  return (uint32_t)(SD.totalBytes() / BYTES_IN_MBYTE);
}
uint32_t StorageDevice::usedMBytes() {
  return (uint32_t)(SD.usedBytes() / BYTES_IN_MBYTE);
}

void StorageDevice::getFilenames(const char* path,
                                 void (*callback)(const char*),
                                 const char* extension) {
  home = SD.open(path);
  while (true) {
    tempFile = home.openNextFile();
    if (!tempFile) {
      // no more files
      break;
    }
    char* filename = new char[middleBufferSize];
    const char* originalFilename = tempFile.name();
    // try to get just filename for cases when tempFile.name() returns full path
    char* relativeFilename = strrchr(originalFilename, '/');

    if (relativeFilename != nullptr) {
      // just to skip first: '/'
      if (strlen(relativeFilename) > 1) {
        relativeFilename++;
      }
      strncpy(filename, relativeFilename, middleBufferSize);
    } else {
      strncpy(filename, tempFile.name(), middleBufferSize);
    }

    if (extension != nullptr) {
      if (hasSuchExtension(filename, extension)) {
        callback(filename);
      }
    } else {
      callback(filename);
    }
    tempFile.close();
  }
  home.close();
}

bool StorageDevice::hasSuchExtension(const char* filename,
                                     const char* extension) {
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
  toLowerCase(_temp_str);

  // copy extension to prevent its modification as well
  strcpy(_small_buffer, extension);
  // and lover case for it too
  toLowerCase(_small_buffer);

  return strstr(_temp_str, _small_buffer) != nullptr;
}

size_t StorageDevice::getFileSize(const char* filepath) {
  tempFile = SD.open(filepath);
  if (!tempFile) {
    debug("E can not open file: %s", filepath);
    return 0;
  }
  size_t size = tempFile.size();
  tempFile.close();
  return size;
}

uint16_t _blockIndex = 0;
bool StorageDevice::readFile(const char* filepath,
                             void (*callback)(uint8_t*, uint16_t, uint16_t,
                                              bool),
                             uint32_t start) {
  if (filepath == nullptr) {
    return false;
  }

  if (SD.exists(filepath) == false) {
    debug("file[%s] not exists!", filepath);
    return false;
  }

  if (isReading == false) {
    isReading = true;
    tempFile = SD.open(filepath);
    if (!tempFile) {
      debug("E can not open file: %s", filepath);
      return false;
    }
    if (start == 0) {
      _blockIndex = 0;
    }

    if (start > 0 && !tempFile.seek(start)) {
      debug("can't seek to pos: %u", start);
      return false;
    } else {
      debug("started read from pos: %u", start);
    }
  } else {
    debug("file in opened already");
  }

  uint16_t bytesRead = 0;
  bytesRead = tempFile.read(_file_buff, hugeBufferSize);
  _blockIndex++;
  debug("read: %u bytes", bytesRead);
  if (bytesRead != hugeBufferSize) {
    isReading = false;
    tempFile.close();
    debug("closing file");
  }
  callback(_file_buff, bytesRead, _blockIndex, bytesRead != hugeBufferSize);

  return true;
}

bool StorageDevice::eraseFlash(size_t fileSize) {
  // adjust size to be aligned to hugeBufferSize (4kb)
  size_t areaToErase = alignTo(fileSize, hugeBufferSize);
  debug("area to erase: %u", areaToErase);

  return spi_flash_erase_range(SPI_FLASH_ADDRESS, areaToErase) == ESP_OK;
}

bool StorageDevice::savePortionToFlash(uint8_t* block, uint16_t blockLength,
                                       uint16_t blockIndex) {
  size_t address = SPI_FLASH_ADDRESS;
  address += blockIndex * hugeBufferSize;
  debug("writing block %u to flash", blockIndex);
  return spi_flash_write(address, block, blockLength) == ESP_OK;
}

uint8_t* StorageDevice::getSavedDataOnFlash(size_t romSize) {
  const void* rom;
  esp_err_t result =
      spi_flash_mmap(SPI_FLASH_ADDRESS, romSize, SPI_FLASH_MMAP_DATA, &rom,
                     &spiFlashMmapHandle);
  ESP_ERROR_CHECK(result);
  debug("mmap_res: handle=%d", spiFlashMmapHandle);
  debug("mmap_res: ptr=%p", rom);
  debug("rom mapped on flash");
  return (uint8_t*)rom;
}