#include "app.h"

int App::getId() { return this->id; }

const char *App::getName() { return this->name; }

bool App::isRunning() { return this->running; }

void App::open(void (*listener)()) {
  if (this->running == true) return;

  this->onCloseListener = listener;
  this->running = true;
  this->needsToRedraw = true;
  init();
}

void App::close() {
  if (this->running == false) return;
  this->running = false;
  this->onClose();
  if (this->onCloseListener != nullptr) {
    this->onCloseListener();
  }
  this->onCloseListener = nullptr;
}

void App::update() {
  if (this->running == false) return;
  onUpdate();
}

void App::draw(DisplayDevice *display) {
  if (this->running == false) return;
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

bool App::handle(JoystickDevice *joystick) {
  if (isRunning() == false) {
    return false;
  }
  return onHandleInput(joystick);
}
