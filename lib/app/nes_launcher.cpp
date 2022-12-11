#include "nes_launcher.h"

#include <controls.h>
#include <display.h>
#include <utils.h>

FileName *filenames = nullptr;
// total count of files
uint16_t fileCount = 0;
// indicates count of files shown on screen
uint8_t fileCountOnScreen = 0;
// start y position on screen for content
uint8_t startPosOnScreenForContent = 48;
// one element height
uint8_t elementHeight = 20;
uint16_t _cursorPosOnScreen;

void printFileNames(FileName *first, uint16_t count);
void drawFileNames(FileName *first, uint16_t offset, uint8_t filesPerPage);
uint16_t getCursorPosOnScreen(uint8_t cursorPos);

void NesLauncher::init() {
  // init nes
  debug("[nes] create nes");
  this->nes = createNes();
  if (this->nes == nullptr) {
    debug("E [nes] can not create nes instance");
    return;
  }
  // fetch *nes files on FS
  debug("[nes] fetching filenames");
  filenames = getAllNesFiles(this->path);
  fileCount = getSize(filenames);
  printFileNames(filenames, fileCount);

  // todo: code here
}

bool NesLauncher::handle(uint16_t keyState) {
  if (this->running == false) {
    return false;
  }

  if (isUpPressed(keyState)) {
    this->cursorPos -= 1;
    if (this->cursorPos < 0) {
      this->cursorPos = fileCountOnScreen;
    }
    requestRedraw();
  }

  if (isDownPressed(keyState)) {
    this->cursorPos += 1;
    if (this->cursorPos > fileCountOnScreen) {
      this->cursorPos = 0;
    }
    requestRedraw();
  }

  if (isStartPressed(keyState) || isSelectPressed(keyState)) {
    close();
  }
  return true;
}

void NesLauncher::onUpdate() {}

void NesLauncher::draw() {
  // title
  nescreen::fillScreen(DARK_GREY);
  nescreen::drawString(64, 10, this->name, WHITE_COLOR, DARK_GREY);

  // path
  nescreen::setTextPosition(16, 24);
  nescreen::drawText(this->path, WHITE_COLOR, DARK_GREY);

  // file names
  drawFileNames(filenames, 0, this->filesPerPage);
  _cursorPosOnScreen = getCursorPosOnScreen(this->cursorPos);

  // cursor
  nescreen::setTextPosition(16, _cursorPosOnScreen);
  nescreen::drawText(">>", WHITE_COLOR, DARK_GREY);

  nescreen::update();
}

void drawFileNames(FileName *first, uint16_t offset, uint8_t filesPerPage) {
  FileName *current = seek(first, offset);
  if (current == nullptr) {
    return;
  }
  uint8_t posOnScreen = startPosOnScreenForContent;
  fileCountOnScreen = 0;
  for (uint8_t index = 0; index < filesPerPage; ++index) {
    nescreen::setTextPosition(40, posOnScreen);
    nescreen::drawText(current->name, WHITE_COLOR, DARK_GREY);

    current = current->next;
    if (current == nullptr) {
      break;
    }
    posOnScreen += elementHeight;
    fileCountOnScreen++;
  }
}

uint16_t getCursorPosOnScreen(uint8_t cursorPos) {
  return startPosOnScreenForContent + cursorPos * elementHeight;
}

// test function
void printFileNames(FileName *first, uint16_t count) {
  if (first == nullptr) {
    return;
  }
  FileName *p = first;
  while (p != nullptr) {
    printf("%s\n", p->name);
    p = p->next;
  }
  printf("total %u files\n", count);
}