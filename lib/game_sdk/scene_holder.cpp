#include "scene_holder.h"

#include <log.h>

SceneHolder::SceneHolder() { this->canvas = new Canvas(); }

SceneHolder::~SceneHolder() {
  for (auto it = sprites.begin(); it != sprites.end(); ++it) {
    delete *it;
  }
  sprites.clear();
  delete canvas;
}

Sprite *SceneHolder::createSprite(uint8_t width, uint8_t height) {
  Sprite *sprite = new Sprite(width, height);
  this->sprites.push_back(sprite);
  return sprite;
}

// step where all sprites are backed into canvas
// after this step canvas can be drawn on screen
void SceneHolder::bakeCanvas() {
  uint16_t index = 0;
  Sprite *sprite = nullptr;
  ColorIndex pixel = 0;
  volatile bool isPixelUsedInSprite = false;

  for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
    for (uint8_t x = 0; x < WIDTH_IN_V_PIXELS; ++x) {
      isPixelUsedInSprite = false;
      for (auto it = sprites.begin(); it != sprites.end(); ++it) {
        sprite = *it;
        if (sprite != nullptr && sprite->contains(x, y) == true) {
          pixel = sprite->getPixel(x, y);
          canvas->setPixel(index, pixel);
          isPixelUsedInSprite = true;
        }
      }
      if (isPixelUsedInSprite == false) {
        canvas->setPixel(index, 0);
      }
      ++index;
    }
  }
}