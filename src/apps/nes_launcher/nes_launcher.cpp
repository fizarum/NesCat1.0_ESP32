#include "nes_launcher.h"
#ifndef PORT_SDK
#include <Esp.h>
#endif  // PORT_SDK
#include <log.h>
#include <string.h>
#include <utils.h>

// FileName *filenames = nullptr;
// total count of files
uint16_t fileCount = 0;
// indicates count of files shown on screen
uint8_t fileCountOnScreen = 0;
// start y position on screen for content
uint8_t startPosOnScreenForContent = 48;
// one element height
uint8_t elementHeight = 20;
uint16_t _cursorPosOnScreen;

// file loading process
bool _readyForNextRead = false;
bool _requestedForNextRead = false;
uint8_t _loadedInPercents = 0;
char _loadingTitle[16];

bool _started = false;

NesLauncher *_launcher;

void onFileLoafingListener(uint8_t percents, bool isLast);
#ifndef PORT_SDK
void getMemoryStatus();
void getPsRamStatus(uint32_t psramSize);
#endif  // PORT_SDK

// void printFileNames(FileName *first, uint16_t count);
// void drawFileNames(DisplayDevice *display, FileName *first, uint16_t offset,
//  uint8_t filesPerPage, uint8_t selectedFileIndex);
// uint16_t getCursorPosOnScreen(uint8_t cursorPos);

void NesLauncher::onOpen() {
  _launcher = this;
  // preparePsRam();
  getMemoryStatus();

  // init nes
  // debug("[nes] create nes");
  // this->nes = createNes();
  // if (this->nes == nullptr) {
  //   debug("E [nes] can not create nes instance");
  //   return;
  // }
  // fetch *nes files on FS
  debug("[nes] fetching filenames");
  // filenames = getAllNesFiles(this->path);
  // fileCount = getSize(filenames);
  // printFileNames(filenames, fileCount);
}

bool NesLauncher::onHandleInput(InputDevice *inputDevice) {
  // if app in process of loading rom - skip handling buttons for a while
  if (this->isLoading == true) {
    debug("[nes] still loading...");
    return true;
  }

  if (inputDevice->isUpKeyDown()) {
    this->cursorPos -= 1;
    if (this->cursorPos < 0) {
      this->cursorPos = fileCountOnScreen;
    }
    requestRedraw();
  }

  if (inputDevice->isDownKeyDown()) {
    this->cursorPos += 1;
    if (this->cursorPos > fileCountOnScreen) {
      this->cursorPos = 0;
    }
    requestRedraw();
  }

  if (inputDevice->isXKeyDown()) {
    if (_started == true) return true;
    _started = true;

    resetLoadingStats(true);

    // get selected filename
    const char *filepath = selectFile();
    if (filepath == nullptr) {
      this->isLoading = false;
      debug("E can not read file under path: %s", filepath);
      return true;
    }

    // pickRomFile(filepath);

    // if (getRomData(filepath, onFileLoafingListener) == false) {
    //   debug("E can't read data!");
    //   resetLoadingStats();
    // }
    requestRedraw();
  }

  return true;
}

void NesLauncher::onUpdate() {
  // for now we're handling loading in onUpdate()
  if (this->isLoading == false) {
    return;
  }

  if (_readyForNextRead == true && _requestedForNextRead == false) {
    _requestedForNextRead = true;
    // read next portion of data
    // if (getRomData(fullPathToSelectedNes, onFileLoafingListener) == false) {
    //   debug("E can't read data!");
    //   resetLoadingStats();
    // }
    requestRedraw();
  }
}

void NesLauncher::onDraw(DisplayDevice *display) {
  // // file names
  // drawFileNames(display, filenames, 0, this->filesPerPage, this->cursorPos);
  // _cursorPosOnScreen = getCursorPosOnScreen(this->cursorPos);

  // // cursor
  // // nescreen::setTextPosition(16, _cursorPosOnScreen);
  // display->drawString(16, _cursorPosOnScreen, ">>", COLOR_WHITE);

  // if (this->isLoading == true) {
  //   sprintf(_loadingTitle, "loaded: %u%%", _loadedInPercents);
  //   display->fillRectangle(0, 50, DEFAULT_WIDTH, 40, COLOR_BLUE);
  //   display->drawString(75, 60, _loadingTitle, COLOR_ORANGE);
  // }
}

void NesLauncher::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_DARKGREY);
}

void NesLauncher::drawTitle(DisplayDevice *display) {
  display->drawString(64, 20, getName(), COLOR_WHITE);
}

const char *NesLauncher::selectFile() {
  // FileName *file = seek(filenames, pageCount * filesPerPage + cursorPos);
  // if (file == nullptr || file->name == nullptr) {
  //   return nullptr;
  // }
  // memset(fullPathToSelectedNes, 0, 256);
  // strcat(fullPathToSelectedNes, path);
  // strcat(fullPathToSelectedNes, "/");
  // strcat(fullPathToSelectedNes, file->name);

  // return fullPathToSelectedNes;
  return nullptr;
}

void NesLauncher::resetLoadingStats(bool startLoading) {
  this->isLoading = startLoading;
  _readyForNextRead = false;
  _requestedForNextRead = false;
}

void onFileLoafingListener(uint8_t percents, bool isFinished) {
  if (isFinished == true) {
    _launcher->resetLoadingStats();
    _launcher->requestRedraw();
  } else {
    _loadedInPercents = percents;
    // indicate that we're ready to read next portion of data
    _readyForNextRead = true;
    _requestedForNextRead = false;
  }
}

// void drawFileNames(DisplayDevice *display, FileName *first, uint16_t offset,
//                    uint8_t filesPerPage, uint8_t selectedFileIndex) {
//   // FileName *current = seek(first, offset);
//   // if (current == nullptr) {
//   //   return;
//   // }
//   // uint8_t posOnScreen = startPosOnScreenForContent;
//   // fileCountOnScreen = 0;

//   // for (uint8_t index = 0; index < filesPerPage; ++index) {
//   //   display->drawString(40, posOnScreen, current->name, COLOR_WHITE);

//   //   current = current->next;
//   //   if (current == nullptr) {
//   //     break;
//   //   }
//   //   posOnScreen += elementHeight;
//   //   fileCountOnScreen++;
//   // }
// }

// uint16_t getCursorPosOnScreen(uint8_t cursorPos) {
//   return startPosOnScreenForContent + cursorPos * elementHeight;
// }

// test function
// void printFileNames(FileName *first, uint16_t count) {
//   // if (first == nullptr) {
//   //   return;
//   // }

//   // FileName *p = first;
//   // while (p != nullptr) {
//   //   printf("%s\n", p->name);
//   //   p = p->next;
//   // }
//   // printf("total %u files\n", count);
// }
#ifndef PORT_SDK
void getMemoryStatus() {
  debug("--------------------------------");
  debug("TOTAL HEAP: %u", ESP.getHeapSize());
  debug("FREE HEAP:: %u", ESP.getFreeHeap());
  uint32_t heapCapsFreeSize = heap_caps_get_free_size(MALLOC_CAP_8BIT);
  debug("heap_caps_get_free_size: %u", heapCapsFreeSize);
  debug("--------------------------------");
}

// uint32_t size = ESP.getPsramSize();
void getPsRamStatus(uint32_t psramSize) {
  debug("--------------------------------");
  if (psramSize > 0) {
    debug("Total PSRAM: %u", psramSize);
    debug("Free PSRAM: %u", ESP.getFreePsram());
  } else {
    debug("NO PSRAM DETECTED.");
  }
  debug("--------------------------------");
}
#endif  // PORT_SDK