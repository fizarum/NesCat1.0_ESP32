#include "app.h"

void App::init(int id, const char *name) {
  this->_id = id;
  this->_name = name;
  this->_running = false;
}

int App::getId() { return this->_id; }

const char *App::getName() { return this->_name; }

bool App::isRunning() { return this->_running; }

void App::open(void (*listener)()) {
  if (this->_running == true) return;

  this->onCloseListener = listener;
  this->_running = true;
  this->needsToRedraw = true;
  onOpen();
}

void App::close() {
  if (this->_running == false) return;
  this->_running = false;
  this->onClose();
  if (this->onCloseListener != nullptr) {
    this->onCloseListener();
  }
  this->onCloseListener = nullptr;
}

void App::update() {
  if (this->_running == false) return;
  onUpdate();
}

void App::draw(DisplayDevice *display) {
  if (this->_running == false) return;
  if (this->needsToBeRedrawn() == false) return;
  if (display == nullptr) return;

  drawBackground(display);
  drawTitle(display);
  drawStatusBar(display);
  onDraw(display);
  this->needsToRedraw = false;
}

void App::requestRedraw() { this->needsToRedraw = true; }

bool App::needsToBeRedrawn() { return this->needsToRedraw; }

bool App::handleInput(InputDevice *inputDevice) {
  if (isRunning() == false) {
    return false;
  }

  if (preventClosingByUser() == false) {
    if (inputDevice->isMenuPressed()) {
      this->close();
      this->requestRedraw();
      return true;
    }
  }

  return onHandleInput(inputDevice);
}
