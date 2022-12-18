#include "nes_wrapper.h"

#include <audio.h>
#include <storage.h>
#include <string.h>
#include <utils.h>

#include "NESemulator_part1.h"
#include "NESemulator_part2.h"
#include "mappers.h"
#include "nes/sndinfo_t.h"

#define NES_RAM_SIZE 0x800
// NTSC = 60Hz, PAL = 50Hz
#define NES_REFRESH_RATE 50

nes_t *instance;

// file loading process
size_t _totalBytes = 0;
size_t _readBytes = 0;
size_t _totalBytesRead = 0;

// untested
uint32_t psRAMSize = 0;
uint8_t *PSRAM;

void onNewFileFoundCallback(const char *fname);
void (*onFileLoadingCallback)(uint8_t percents, bool isFinished);

void onFileLoafingListener(uint8_t *block, uint16_t blockLength,
                           uint16_t blockIndex, bool isLast);

size_t getRomFileSize(const char *filepath);
void prepareFlash(size_t sizeToPrepare);
bool storeRomDataOnFlash(uint8_t *block, uint16_t blockSize,
                         uint16_t blockIndex);
void resetLoadingStats();

nes_t *createNes() {
  if (instance == nullptr) {
    instance = nes_create();
  }
  if (instance == nullptr) {
    debug("can not create nes instance");
  }
  return instance;
}

FileName *_first = nullptr;
FileName *_last = nullptr;

FileName *getAllNesFiles(const char *path) {
  cleanup(_first);
  _first = nullptr;
  _last = nullptr;
  storageInit();

  getFilenames(path, onNewFileFoundCallback, "nes");
  return _first;
}

void pickRomFile(const char *filepath) {
  resetLoadingStats();
  debug("selected file: %s", filepath);
  _totalBytes = getRomFileSize(filepath);
  debug("file size: %u", _totalBytes);
  prepareFlash(_totalBytes);
}

size_t getRomFileSize(const char *filepath) { return getFileSize(filepath); }

void prepareFlash(size_t sizeToPrepare) {
  if (eraseFlash(sizeToPrepare) == true) {
    debug("flash erased!");
  } else {
    debug("E can't prepare flash!");
  }
}

/** void (*callback)(uint8_t) has percents as an argument */
bool getRomData(const char *filepath, void (*callback)(uint8_t, bool)) {
  onFileLoadingCallback = callback;
  bool result = readFile(filepath, onFileLoafingListener);
  if (result == false) {
    resetLoadingStats();
  }
  return result;
}

bool storeRomDataOnFlash(uint8_t *block, uint16_t blockSize,
                         uint16_t blockIndex) {
  return savePortionToFlash(block, blockSize, blockIndex);
}

void onNewFileFoundCallback(const char *fname) {
  FileName *f = createFileName(fname);
  if (_first == nullptr) {
    _first = f;
    _last = f;
    return;
  }
  _last->next = f;
  f->prev = _last;
  _last = f;
}

void onFileLoafingListener(uint8_t *block, uint16_t blockLength,
                           uint16_t blockIndex, bool isLast) {
  _readBytes = blockLength;
  _totalBytesRead += _readBytes;

  // store data...
  storeRomDataOnFlash(block, blockLength, blockIndex);

  // on loading finished
  if (isLast == true) {
    debug("file loading finished: %u", _totalBytesRead);
    onFileLoadingCallback(100, true);
    resetLoadingStats();
    // storage::getSavedDataOnFlash()
  } else {
    debug("file, read total bytes: %u", _totalBytesRead);
    onFileLoadingCallback(_totalBytesRead * 100 / _totalBytes, false);
  }
}

void resetLoadingStats() {
  _readBytes = 0;
  _totalBytesRead = 0;
}

void preparePsRam() {
  psRAMSize = ESP.getPsramSize();
  getPsRamStatus(psRAMSize);

  if (psRAMSize > 0) {
    PSRAM = (uint8_t *)ps_malloc(2097152);  // PSRAM malloc 2MB
  }
}