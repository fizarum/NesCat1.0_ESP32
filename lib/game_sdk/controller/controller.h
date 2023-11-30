#ifndef GAME_SDK_CONTROLLER_H
#define GAME_SDK_CONTROLLER_H

#include <display_device/display_device.h>
#include <scheduler.h>

#include <vector>

#include "canvas.h"
#include "sprite.h"

class Controller {
 private:
  static const uint8_t renderTaskPriority = 2;
  static const uint8_t fbTaskPriority = 1;

  volatile bool isLoopStarted = false;
  std::vector<Sprite *> sprites = {};
  Canvas *canvas;
  Palette *spritePalette;

  TaskHandle_t renderTaskHandler;
  TaskHandle_t frameBufferTaskHandler;

 public:
  Controller();
  ~Controller();

  void startLoop(DisplayDevice *_display);
  void stopLoop();

  Sprite *createSprite();
};

#endif  // GAME_SDK_CONTROLLER_H