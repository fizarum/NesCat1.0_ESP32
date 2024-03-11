#ifndef GAME_SDK_GAME_OBJECT_H
#define GAME_SDK_GAME_OBJECT_H

#include "sprite.h"

typedef uint16_t ObjectId;

class GameObject {
 private:
  ObjectId spriteId;
  bool isCollidable;
  bool isGravitable;

 public:
  GameObject(ObjectId spriteId, bool isCollidable = false,
             bool isGravitable = false) {
    this->spriteId = spriteId;
    this->isCollidable = isCollidable;
    this->isGravitable = isGravitable;
  }

  ObjectId getSpriteId() { return spriteId; }
};

#endif  // GAME_SDK_GAME_OBJECT_H