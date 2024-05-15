#include "scene_holder.h"

#include "../log/log.h"

SceneHolder *__self = nullptr;
void (*__callback)(uint8_t x, uint8_t y, Color color) = nullptr;
void __onEachDurtyLine(DRTLine_t *line, uint8_t lineNumber);

SceneHolder::SceneHolder(Palette_t *palette,
                         void (*onPixelUpdatedCallback)(uint8_t x, uint8_t y,
                                                        Color color)) {
  this->palette = palette;
  this->tracker = DRTrackerCreate();
  this->holder = SpritesHolderCreate();

  __callback = onPixelUpdatedCallback;
  __self = this;
}

SceneHolder::~SceneHolder() {
  SpritesHolderDestroy(this->holder);

  for (auto &it : gameObjectsWithId) {
    GameObjectDestroy(it.second);
  }

  for (auto &it : backgroundGameObjectsWithId) {
    GameObjectDestroy(it.second);
  }

  gameObjectsWithId.clear();
  backgroundGameObjectsWithId.clear();

  DRTrackerDestroy(tracker);
}

ObjectId SceneHolder::createSprite(_u8 width, _u8 height, ColorIndex pixels[],
                                   size_t pixelsCount, _u8 x, _u8 y) {
  SpriteType_t spriteType = SPRITE_TYPE_FOREGROUND;

  ObjectId id = SpritesHolderAddSprite(this->holder, width, height, pixels,
                                       pixelsCount, spriteType, x, y);
  Sprite_t *sprite = (Sprite_t *)id;
  Rectangle_t *bounds = SpriteGetBounds(sprite);
  setDurtyRegion(bounds);
  return id;
}

ObjectId SceneHolder::createAnimatedSprite(
    uint8_t width, uint8_t height, ColorIndex *const pixels, size_t pixelsCount,
    const AnimationSpeed_t animationSpeed, const uint8_t x, const uint8_t y) {
  SpriteType_t spriteType = SPRITE_TYPE_FOREGROUND;

  ObjectId id = SpritesHolderAddAnimatedSprite(this->holder, width, height,
                                               pixels, pixelsCount, spriteType,
                                               animationSpeed, x, y);
  AnimatedSprite_t *sprite = (AnimatedSprite_t *)id;
  Rectangle_t *bounds = AnimatedSpriteGetBounds(sprite);
  setDurtyRegion(bounds);
  return id;
}

ObjectId SceneHolder::createBackgroundSprite(_u8 width, _u8 height,
                                             ColorIndex pixels[],
                                             size_t pixelsCount, _u8 x, _u8 y) {
  SpriteType_t spriteType = SPRITE_TYPE_BACKGROUND;

  ObjectId id = SpritesHolderAddSprite(this->holder, width, height, pixels,
                                       pixelsCount, spriteType, x, y);
  Sprite_t *sprite = (Sprite_t *)id;
  Rectangle_t *bounds = SpriteGetBounds(sprite);
  setDurtyRegion(bounds);
  return id;
}

ObjectId SceneHolder::createGameObject(uint8_t width, uint8_t height,
                                       ColorIndex pixels[], size_t pixelsCount,
                                       bool isCollidable, bool isObstacle,
                                       bool isGravitable) {
  ObjectId id = this->createSprite(width, height, pixels, pixelsCount, 0, 0);

  GameObject_t *object =
      GameObjectCreate(id, isCollidable, isObstacle, isGravitable);
  ObjectId oid = (ObjectId)object;
  gameObjectsWithId[oid] = object;
  // todo rework this
  return oid;
}

ObjectId SceneHolder::createBackgroundGameObject(
    uint8_t width, uint8_t height, ColorIndex pixels[], size_t pixelsCount,
    bool isCollidable, bool isObstacle, bool isGravitable) {
  ObjectId id =
      this->createBackgroundSprite(width, height, pixels, pixelsCount, 0, 0);

  GameObject_t *object =
      GameObjectCreate(id, isCollidable, isObstacle, isGravitable);

  backgroundGameObjectsWithId[id] = object;

  return (ObjectId)object;
}

void SceneHolder::moveSpriteBy(ObjectId spriteId, int8_t x, int8_t y) {
  Sprite_t *sprite = (Sprite_t *)spriteId;

  // set old region as durty
  setDurtyRegion(SpriteGetBounds(sprite));
  SpriteMoveBy(sprite, x, y);
  // as well as new
  setDurtyRegion(SpriteGetBounds(sprite));
}

void SceneHolder::moveSpriteTo(ObjectId spriteId, int8_t x, int8_t y) {
  Sprite_t *sprite = (Sprite_t *)spriteId;
  // set old region as durty
  setDurtyRegion(SpriteGetBounds(sprite));
  SpriteMoveTo(sprite, x, y);
  // as well as new
  setDurtyRegion(SpriteGetBounds(sprite));
}

void SceneHolder::calculateNextPosition(GameObject_t *object, int8_t moveByX,
                                        int8_t moveByY) {
  uint8_t nextX, nextY = 0;

  Sprite_t *sprite = getSprite(object);

  Direction direction = getDirection(moveByX, moveByY);
  switch (direction) {
    case DIRECTION_LT:
      nextX = SpriteGetVisibleLeftPosition(sprite) + moveByX;
      nextY = SpriteGetVisibleTopPosition(sprite) + moveByY;
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);
      GameObjectResetNextPositionOfCorner2(object);
      break;

    case DIRECTION_T:
      nextX = SpriteGetVisibleLeftPosition(sprite);
      nextY = SpriteGetVisibleTopPosition(sprite) + moveByY;
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);

      nextX = SpriteGetVisibleRightPosition(sprite);
      GameObjectSetNextPositionForCorner2(object, nextX, nextY);
      break;

    case DIRECTION_RT:
      nextX = SpriteGetVisibleRightPosition(sprite) + moveByX;
      nextY = SpriteGetVisibleTopPosition(sprite) + moveByY;
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);
      GameObjectResetNextPositionOfCorner2(object);
      break;

    case DIRECTION_L:
      nextX = SpriteGetVisibleLeftPosition(sprite) + moveByX;
      nextY = SpriteGetVisibleTopPosition(sprite);
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);

      nextY = SpriteGetVisibleBottomPosition(sprite);
      GameObjectSetNextPositionForCorner2(object, nextX, nextY);
      break;

    case DIRECTION_R:
      nextX = SpriteGetVisibleRightPosition(sprite) + moveByX;
      nextY = SpriteGetVisibleTopPosition(sprite);
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);

      nextY = SpriteGetVisibleBottomPosition(sprite);
      GameObjectSetNextPositionForCorner2(object, nextX, nextY);
      break;

    case DIRECTION_LB:
      nextX = SpriteGetVisibleLeftPosition(sprite) + moveByX;
      nextY = SpriteGetVisibleBottomPosition(sprite) + moveByY;
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);
      GameObjectResetNextPositionOfCorner2(object);
      break;

    case DIRECTION_B:
      nextX = SpriteGetVisibleLeftPosition(sprite);
      nextY = SpriteGetVisibleBottomPosition(sprite) + moveByY;
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);

      nextX = SpriteGetVisibleRightPosition(sprite);
      GameObjectSetNextPositionForCorner2(object, nextX, nextY);
      break;

    case DIRECTION_RB:
      nextX = SpriteGetVisibleRightPosition(sprite) + moveByX;
      nextY = SpriteGetVisibleBottomPosition(sprite) + moveByY;
      GameObjectSetNextPositionForCorner1(object, nextX, nextY);
      GameObjectResetNextPositionOfCorner2(object);
      break;

    default:
      GameObjectResetNextPositionOfCorner1(object);
      GameObjectResetNextPositionOfCorner2(object);
      break;
  }
}

ObjectId SceneHolder::getObstacle(GameObject_t *object) {
  const Point_t *nextPositionForCorner1 =
      GameObjectGetNextPositionOfCorner1(object);
  const Point_t *nextPositionForCorner2 =
      GameObjectGetNextPositionOfCorner2(object);

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
  GameObject_t *objectToMove = this->getGameObject(id);
  if (objectToMove == nullptr) return;

  // if object to move isn't collidable, just move it
  if (GameObjectIsCollidable(objectToMove) == false) {
    ObjectId spriteId = GameObjectGetSpriteId(objectToMove);
    this->moveSpriteBy(spriteId, x, y);
    return;
  }

  calculateNextPosition(objectToMove, x, y);
  ObjectId anotherObjectId = getObstacle(objectToMove);

  // if no obstacles detected - move object
  if (anotherObjectId == OBJECT_ID_UNDEF) {
    ObjectId spriteId = GameObjectGetSpriteId(objectToMove);

    this->moveSpriteBy(spriteId, x, y);
    return;
  }

  GameObject_t *anotherObject = this->getGameObject(anotherObjectId);
  if (anotherObject == nullptr) return;

  if (GameObjectIsCollidable(anotherObject) == true) {
    debug("hit registered with object: %d\n", anotherObjectId);
    // if anotherObject is obstacle - stop movement
    if (GameObjectIsObstacle(anotherObject) == false) {
      ObjectId spriteId = GameObjectGetSpriteId(anotherObject);
      this->moveSpriteBy(spriteId, x, y);
    }
  }
}

void SceneHolder::moveGameObjectTo(ObjectId id, int8_t x, int8_t y) {
  GameObject_t *object = this->getGameObject(id);

  if (object == nullptr) return;
  ObjectId spriteId = GameObjectGetSpriteId(object);
  this->moveSpriteTo(spriteId, x, y);
}

ColorIndex SceneHolder::findPixelInGameObjects(uint8_t x, uint8_t y,
                                               ColorIndex defaultColorIndex) {
  for (auto &it : gameObjectsWithId) {
    Sprite_t *sprite = getSprite(it.second);
    if (sprite != nullptr) {
      ColorIndex ci = SpriteGetPixel(sprite, x, y, defaultColorIndex);
      if (PaletteIsColorVisible(palette, ci) == true) {
        return ci;
      }
    }
  }
  for (auto &it : backgroundGameObjectsWithId) {
    Sprite_t *sprite = getSprite(it.second);
    if (sprite != nullptr) {
      ColorIndex ci = SpriteGetPixel(sprite, x, y, defaultColorIndex);
      if (PaletteIsColorVisible(palette, ci) == true) {
        return ci;
      }
    }
  }

  return defaultColorIndex;
}

ColorIndex SceneHolder::findPixelInSprites(_u8 x, _u8 y,
                                           ColorIndex defaultColorIndex) {
  SpriteType_t type = SPRITE_TYPE_FOREGROUND;
  ObjectId id = SpritesHolderGetSprite(holder, type, false, x, y);

  if (id != OBJECT_ID_NA) {
    Sprite_t *sprite = (Sprite_t *)id;
    ColorIndex ci = SpriteGetPixel(sprite, x, y, defaultColorIndex);
    if (PaletteIsColorVisible(palette, ci) == true) {
      return ci;
    }
  }
  return defaultColorIndex;
}

ColorIndex SceneHolder::findPixelInAnimatedSprites(
    _u8 x, _u8 y, ColorIndex defaultColorIndex) {
  SpriteType_t type = SPRITE_TYPE_FOREGROUND;
  ObjectId id = SpritesHolderGetSprite(holder, type, true, x, y);

  if (id != OBJECT_ID_NA) {
    AnimatedSprite_t *sprite = (AnimatedSprite_t *)id;
    ColorIndex ci = AnimatedSpriteGetPixel(sprite, x, y, defaultColorIndex);
    if (ci != defaultColorIndex) {
      return ci;
    }
  }
  return defaultColorIndex;
}

ColorIndex SceneHolder::findPixelInBackgroundSprites(
    uint8_t x, uint8_t y, ColorIndex defaultColorIndex) {
  SpriteType_t type = SPRITE_TYPE_BACKGROUND;

  ObjectId id = SpritesHolderGetSprite(holder, type, false, x, y);
  if (id != OBJECT_ID_NA) {
    Sprite_t *sprite = (Sprite_t *)id;
    ColorIndex ci = SpriteGetPixel(sprite, x, y, defaultColorIndex);
    if (ci != defaultColorIndex) {
      return ci;
    }
  }
  return defaultColorIndex;
}

ObjectId SceneHolder::findObjectOnScreen(uint8_t x, uint8_t y) {
  for (auto &it : gameObjectsWithId) {
    Sprite_t *sprite = getSprite(it.second);
    if (sprite != nullptr && SpriteContainsPoint(sprite, x, y)) {
      return it.first;
    }
  }

  for (auto &it : backgroundGameObjectsWithId) {
    Sprite_t *sprite = getSprite(it.second);
    if (sprite != nullptr && SpriteContainsPoint(sprite, x, y)) {
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

  // and animated sprites
  colorIndex = findPixelInAnimatedSprites(x, y, backgroundColorIndex);
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

  DRTrackerGetDurtyRegions(tracker, &__onEachDurtyLine);
  DRTrackerClear(tracker);
}

void __updateAnimationStateCallback(AnimatedSprite_t *sprite) {
  AnimatedSpriteUpdateState(sprite);
  bool isNewFrame = AnimatedSpriteIsFrameChanged(sprite);
  if (isNewFrame == true) {
    Rectangle_t *bounds = AnimatedSpriteGetBounds(sprite);
    __self->setDurtyRegion(bounds);
  }
}

void SceneHolder::updateAnimationState() {
  SpritesHolderForeachAnimatedSprite(holder, SPRITE_TYPE_BACKGROUND,
                                     __updateAnimationStateCallback);

  SpritesHolderForeachAnimatedSprite(holder, SPRITE_TYPE_FOREGROUND,
                                     __updateAnimationStateCallback);

  SpritesHolderForeachAnimatedSprite(holder, SPRITE_TYPE_HUD,
                                     __updateAnimationStateCallback);
}

void SceneHolder::setDurtyRegion(const Rectangle_t *region) {
  uint8_t left = RectangleGetVisibleLeftPosition(region);
  uint8_t top = RectangleGetVisibleTopPosition(region);
  uint8_t right = RectangleGetVisibleRightPosition(region);
  uint8_t bottom = RectangleGetVisibleBottomPosition(region);
  DRTrackerSetDurtyRegion(tracker, left, top, right, bottom);
}

void SceneHolder::drawDurtyRegion() {
#ifdef PORT_SDK
  DRTrackerPrintDebugInfo(tracker);
#endif
}