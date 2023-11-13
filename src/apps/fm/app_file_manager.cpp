#include "app_file_manager.h"

#include <device_manager.h>
#include <filename.h>
#include <log.h>

#include <vector>

#include "devices/storage/storage_device.h"

void _onFileFoundCallback(const char *fname);
void _drawFileNames(DisplayDevice *display, uint16_t offset,
                    uint8_t filesPerPage, uint8_t selectedFileIndex);
uint16_t _getCursorPosOnScreen(uint8_t cursorPos);

// one element height
const uint8_t elementHeight = 20;
// start y position on screen for content
const uint8_t startPosOnScreenForContent = 48;
const uint8_t filesPerPage = 8;

// indicates count of files shown on screen
uint8_t _fileCountOnScreen = 0;

// list of filenames
std::vector<FileName *> _filenames = {};

void FileManager::onOpen() {
  StorageDevice *storage =
      (StorageDevice *)DeviceManager::get(DEVICE_STORAGE_ID);
  storage->getFilenames("/", _onFileFoundCallback);
}

bool FileManager::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isUpPressed()) {
    this->cursorPos -= 1;
    if (this->cursorPos < 0) {
      this->cursorPos = _fileCountOnScreen - 1;
    }

    requestRedraw();
  }

  if (inputDevice->isDownPressed()) {
    this->cursorPos += 1;
    if (this->cursorPos >= _fileCountOnScreen) {
      this->cursorPos = 0;
    }
    requestRedraw();
  }

  return true;
}

void FileManager::onDraw(DisplayDevice *display) {
  // file names
  _drawFileNames(display, 0, filesPerPage, this->cursorPos);
  this->cursorPosOnScreen = _getCursorPosOnScreen(this->cursorPos);

  // cursor
  display->drawString(16, this->cursorPosOnScreen, ">>", COLOR_WHITE);
}

void FileManager::drawBackground(DisplayDevice *display) {
  display->fillScreen(COLOR_OLIVE);
}

void FileManager::drawTitle(DisplayDevice *display) {
  display->drawString(64, 20, getName(), COLOR_WHITE);
}

void FileManager::onClose() {
  _filenames.clear();
  cursorPosOnScreen = 0;
  cursorPos = 0;
}

void _onFileFoundCallback(const char *fname) {
  _filenames.push_back(new FileName(fname));
}

void _drawFileNames(DisplayDevice *display, uint16_t offset,
                    uint8_t filesPerPage, uint8_t selectedFileIndex) {
  if (offset >= _filenames.size()) return;

  uint8_t posOnScreen = startPosOnScreenForContent;
  uint8_t drawnFilesOnScreen = 0;

  for (auto it = _filenames.begin() + offset; it != _filenames.end(); ++it) {
    FileName *filename = *it;
    display->drawString(40, posOnScreen, filename->getName(), COLOR_WHITE);
    posOnScreen += elementHeight;
    drawnFilesOnScreen++;
    if (drawnFilesOnScreen > filesPerPage) break;
  }
  _fileCountOnScreen = drawnFilesOnScreen;
}

uint16_t _getCursorPosOnScreen(uint8_t cursorPos) {
  return startPosOnScreenForContent + cursorPos * elementHeight;
}