#include "scene_holder.h"

#include "../log/log.h"

SceneHolder *__self = nullptr;
void (*__callback)(uint8_t x, uint8_t y, Color color) = nullptr;
void __onEachDurtyLine(DRTLine_t *line, uint8_t lineNumber);

SceneHolder::SceneHolder(Palette_t *palette,
                         void (*onPixelUpdatedCallback)(uint8_t x, uint8_t y,
                                                        Color color)) {
  this->palette = palette;
  this->tracker = new DurtyRegionTracker();
  this->lastAssignedId = 0;

  __callback = onPixelUpdatedCallback;
  __self = this;
}

SceneHolder::~SceneHolder() {
  for (auto &it : spritesWithId) {
    delete it.second;
  }

  for (auto &it : backgroundSpritesWithId) {
    delete it.second;
  }

  for (auto &it : gameObjectsWithId) {
    delete it.second;
  }

  for (auto &it : backgroundGameObjectsWithId) {
    delete it.second;
  }

  spritesWithId.clear();
  backgroundSpritesWithId.clear();

  gameObjectsWithId.clear();
  backgroundGameObjectsWithId.clear();

  delete tracker;
}

Sprite *SceneHolder::createPlainSprite(uint8_t width, uint8_t height,
                                       ColorIndex pixels[], size_t pixelsCount,
                                       uint8_t positionX, uint8_t positionY) {
  Sprite *sprite = new Sprite(width, height);
  sprite->setPixels(pixels, pixelsCount);
  if (positionX != 0 || positionY != 0) {
    sprite->moveTo(positionX, positionY);
  }
  setDurtyRegion(sprite);
  return sprite;
}

ObjectId SceneHolder::createSprite(uint8_t width, uint8_t height,
                                   ColorIndex pixels[], size_t pixelsCount,
                                   uint8_t positionX, uint8_t positionY) {
  lastAssignedId++;
  spritesWithId[lastAssignedId] = createPlainSprite(
      width, height, pixels, pixelsCount, positionX, positionY);

  return lastAssignedId;
}

ObjectId SceneHolder::createBackgroundSprite(uint8_t width, uint8_t height,
                                             ColorIndex pixels[],
                                             size_t pixelsCount,
                                             uint8_t positionX,
                                             uint8_t positionY) {
  lastAssignedId++;
  backgroundSpritesWithId[lastAssignedId] = createPlainSprite(
      width, height, pixels, pixelsCount, positionX, positionY);

  return lastAssignedId;
}

ObjectId SceneHolder::createGameObject(uint8_t width, uint8_t height,
                                       ColorIndex pixels[], size_t pixelsCount,
                                       bool isCollidable, bool isObstacle,
                                       bool isGravitable) {
  ObjectId id = this->createSprite(width, height, pixels, pixelsCount, 0, 0);

  lastAssignedId++;
  gameObjectsWithId[lastAssignedId] =
      new GameObject(id, isCollidable, isObstacle, isGravitable);

  return lastAssignedId;
}

ObjectId SceneHolder::createBackgroundGameObject(
    uint8_t width, uint8_t height, ColorIndex pixels[], size_t pixelsCount,
    bool isCollidable, bool isObstacle, bool isGravitable) {
  ObjectId id =
      this->createBackgroundSprite(width, height, pixels, pixelsCount, 0, 0);

  lastAssignedId++;

  backgroundGameObjectsWithId[lastAssignedId] =
      new GameObject(id, isCollidable, isObstacle, isGravitable);

  return lastAssignedId;
}

void SceneHolder::moveSpriteBy(ObjectId spriteId, int8_t x, int8_t y) {
  Sprite *sprite = this->getSprite(spriteId);

  // set old region as durty
  setDurtyRegion(sprite);
  sprite->moveBy(x, y);
  // as well as new
  setDurtyRegion(sprite);
}

void SceneHolder::moveSpriteTo(ObjectId spriteId, int8_t x, int8_t y) {
  Sprite *sprite = this->getSprite(spriteId);
  // set old region as durty
  setDurtyRegion(sprite);
  sprite->moveTo(x, y);
  // as well as new
  setDurtyRegion(sprite);
}

void SceneHolder::calculateNextPosition(GameObject *object, int8_t moveByX,
                                        int8_t moveByY) {
  uint8_t nextX, nextY = 0;

  Sprite *sprite = getSprite(object);

  Direction direction = getDirection(moveByX, moveByY);
  switch (direction) {
    case DIRECTION_LT:
      nextX = sprite->getVisibleLeft() + moveByX;
      nextY = sprite->getVisibleTop() + moveByY;
      object->setNextPositionForCorner1(nextX, nextY);
      object->resetNextPositionOfCorner2();
      break;

    case DIRECTION_T:
      nextX = sprite->getVisibleLeft();
      nextY = sprite->getVisibleTop() + moveByY;
      object->setNextPositionForCorner1(nextX, nextY);

      nextX = sprite->getVisibleRight();
      object->setNextPositionForCorner2(nextX, nextY);
      break;

    case DIRECTION_RT:
      nextX = sprite->getVisibleRight() + moveByX;
      nextY = sprite->getVisibleTop() + moveByY;
      object->setNextPositionForCorner1(nextX, nextY);
      object->resetNextPositionOfCorner2();
      break;

    case DIRECTION_L:
      nextX = sprite->getVisibleLeft() + moveByX;
      nextY = sprite->getVisibleTop();
      object->setNextPositionForCorner1(nextX, nextY);

      nextY = sprite->getVisibleBottom();
      object->setNextPositionForCorner2(nextX, nextY);
      break;

    case DIRECTION_R:
      nextX = sprite->getVisibleRight() + moveByX;
      nextY = sprite->getVisibleTop();
      object->setNextPositionForCorner1(nextX, nextY);

      nextY = sprite->getVisibleBottom();
      object->setNextPositionForCorner2(nextX, nextY);
      break;

    case DIRECTION_LB:
      nextX = sprite->getVisibleLeft() + moveByX;
      nextY = sprite->getVisibleBottom() + moveByY;
      object->setNextPositionForCorner1(nextX, nextY);
      object->resetNextPositionOfCorner2();
      break;

    case DIRECTION_B:
      nextX = sprite->getVisibleLeft();
      nextY = sprite->getVisibleBottom() + moveByY;
      object->setNextPositionForCorner1(nextX, nextY);

      nextX = sprite->getVisibleRight();
      object->setNextPositionForCorner2(nextX, nextY);
      break;

    case DIRECTION_RB:
      nextX = sprite->getVisibleRight() + moveByX;
      nextY = sprite->getVisibleBottom() + moveByY;
      object->setNextPositionForCorner1(nextX, nextY);
      object->resetNextPositionOfCorner2();
      break;

    default:
      object->resetNextPositionOfCorner1();
      object->resetNextPositionOfCorner2();
      break;
  }
}

ObjectId SceneHolder::getObstacle(GameObject *object) {
  Point_t *nextPositionForCorner1 = object->getNextPositionOfCorner1();
  Point_t *nextPositionForCorner2 = object->getNextPositionOfCorner2();
  // check obstacle on corner 1
  ObjectId obstacleId =
      findObjectOnScreen(nextPositionForCorner1->x, nextPositionForCorner1->y);
  // if no obstacle found check another corner
  if (obstacleId == OBJECT_ID_UNDEF) {
    obstacleId = findObjectOnScreen(nextPositionForCorner2->x,
                                    nextPositionForCorner2->y);
  }
  return obstacleId;
}

void SceneHolder::moveGameObjectBy(ObjectId id, int8_t x, int8_t y) {
  GameObject *objectToMove = this->getGameObject(id);
  if (objectToMove == nullptr) return;

  // if object to move isn't collidable, just move it
  if (objectToMove->isCollidable() == false) {
    this->moveSpriteBy(objectToMove->getSpriteId(), x, y);
    return;
  }

  calculateNextPosition(objectToMove, x, y);
  ObjectId anotherObjectId = getObstacle(objectToMove);

  // if no obstacles detected - move object
  if (anotherObjectId == OBJECT_ID_UNDEF) {
    this->moveSpriteBy(objectToMove->getSpriteId(), x, y);
    return;
  }

  GameObject *anotherObject = this->getGameObject(anotherObjectId);
  if (anotherObject == nullptr) return;

  if (anotherObject->isCollidable() == true) {
    debug("hit registered with object: %d\n", anotherObjectId);
    // if anotherObject is obstacle - stop movement
    if (anotherObject->isObstacle() == false) {
      this->moveSpriteBy(objectToMove->getSpriteId(), x, y);
    }
  }
}

void SceneHolder::moveGameObjectTo(ObjectId id, int8_t x, int8_t y) {
  GameObject *object = this->getGameObject(id);

  if (object == nullptr) return;
  this->moveSpriteTo(object->getSpriteId(), x, y);
}

ColorIndex SceneHolder::findPixelInGameObjects(uint8_t x, uint8_t y,
                                               ColorIndex defaultColorIndex) {
  for (auto &it : gameObjectsWithId) {
    Sprite *sprite = getSprite(it.second);
    if (sprite != nullptr) {
      ColorIndex ci = sprite->getPixel(x, y, defaultColorIndex);
      if (PaletteIsColorVisible(palette, ci) == true) {
        return ci;
      }
    }
  }
  for (auto &it : backgroundGameObjectsWithId) {
    Sprite *sprite = getSprite(it.second);
    if (sprite != nullptr) {
      ColorIndex ci = sprite->getPixel(x, y, defaultColorIndex);
      if (PaletteIsColorVisible(palette, ci) == true) {
        return ci;
      }
    }
  }

  return defaultColorIndex;
}

ColorIndex SceneHolder::findPixelInSprites(uint8_t x, uint8_t y,
                                           ColorIndex defaultColorIndex) {
  for (auto &it : spritesWithId) {
    ColorIndex ci = it.second->getPixel(x, y, defaultColorIndex);
    // second check allows to continue searching in siblin sprites
    // when current one has transparent pixel
    if (PaletteIsColorVisible(palette, ci) == true) {
      return ci;
    }
  }
  return defaultColorIndex;
}

ColorIndex SceneHolder::findPixelInBackgroundSprites(
    uint8_t x, uint8_t y, ColorIndex defaultColorIndex) {
  for (auto &it : backgroundSpritesWithId) {
    ColorIndex ci = it.second->getPixel(x, y, defaultColorIndex);
    if (ci != defaultColorIndex) {
      return ci;
    }
  }
  return defaultColorIndex;
}

ObjectId SceneHolder::findObjectOnScreen(uint8_t x, uint8_t y) {
  for (auto &it : gameObjectsWithId) {
    Sprite *sprite = getSprite(it.second);
    if (sprite != nullptr && sprite->contains(x, y)) {
      return it.first;
    }
  }

  for (auto &it : backgroundGameObjectsWithId) {
    Sprite *sprite = getSprite(it.second);
    if (sprite != nullptr && sprite->contains(x, y)) {
      return it.first;
    }
  }
  return OBJECT_ID_UNDEF;
}

Color SceneHolder::calculatePixel(uint8_t x, uint8_t y) {
  ColorIndex backgroundColorIndex = PalettegetBackgroundColor(palette);

  // check game object's sprites
  ColorIndex colorIndex = findPixelInGameObjects(x, y, backgroundColorIndex);
  if (PaletteIsColorVisible(palette, colorIndex) == true) {
    return PaletteGetColor(palette, colorIndex);
  }

  // else foreground sprites
  colorIndex = findPixelInSprites(x, y, backgroundColorIndex);
  if (PaletteIsColorVisible(palette, colorIndex) == true) {
    return PaletteGetColor(palette, colorIndex);
  }

  // nothing found? take background once
  colorIndex = findPixelInBackgroundSprites(x, y, backgroundColorIndex);
  if (PaletteIsColorVisible(palette, colorIndex) == true) {
    // if pixel not taken even by background sprite, take def value
    return PaletteGetColor(palette, colorIndex);
  }

  // if pixel not taken even by background sprite, take def value
  return PalettegetBackgroundColor(palette);
}

void __onEachDurtyLine(DRTLine_t *line, uint8_t lineNumber) {
  for (uint8_t x = 0; x < WIDTH_IN_V_PIXELS; ++x) {
    if (DRTLineIsPixelSet(line, x)) {
      Color color = __self->calculatePixel(x, lineNumber);
      __callback(x, lineNumber, color);
    }
  }
}

void SceneHolder::bakeCanvas() {
  if (__callback == nullptr) return;

  tracker->redrawDurtyPixels(&__onEachDurtyLine);
}

void SceneHolder::setDurtyRegion(uint8_t left, uint8_t top, uint8_t right,
                                 uint8_t bottom) {
  tracker->setDurtyRegion(left, top, right, bottom);
}

void SceneHolder::setDurtyRegion(Rectangle *region, uint8_t extraSpace) {
  // TODO: complete bouds checkings
  uint8_t left = region->getVisibleLeft();
  uint8_t top = region->getVisibleTop();
  uint8_t right = region->getVisibleRight();
  uint8_t bottom = region->getVisibleBottom();

  tracker->setDurtyRegion(left, top, right, bottom);
}

void SceneHolder::removeAllDurtyRegions() { tracker->resetDurtyRegions(); }

void SceneHolder::drawDurtyRegion() {
  // tracker->printDebugInfo();
}