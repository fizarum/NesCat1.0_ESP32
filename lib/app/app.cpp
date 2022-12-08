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
  if (this->onCloseListener != nullptr) {
    this->onCloseListener();
  }
  this->onCloseListener = nullptr;
}

void App::update() {
  if (this->running == false) return;
  onUpdate();

  if (this->needsToBeRedrawn() == true) {
    draw();
    this->needsToRedraw = false;
  }
}

void App::requestRedraw() { this->needsToRedraw = true; }

bool App::needsToBeRedrawn() { return this->needsToRedraw; }
