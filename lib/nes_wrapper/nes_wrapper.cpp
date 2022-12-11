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

void onNewFileFoundCallback(const char *fname);

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
