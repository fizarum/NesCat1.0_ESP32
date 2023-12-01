#ifndef GAME_SDK_CONTROLLER_H
#define GAME_SDK_CONTROLLER_H

#include <display_device/display_device.h>
#include <scheduler.h>

#include <vector>

#include "canvas.h"
#include "sprite.h"

class SceneHolder {
 public:
  std::vector<Sprite *> sprites = {};
  Canvas *canvas;

  SceneHolder();
  ~SceneHolder();

  Sprite *createSprite(uint8_t width, uint8_t height);
  void bakeCanvas();
};

#endif  // GAME_SDK_CONTROLLER_H