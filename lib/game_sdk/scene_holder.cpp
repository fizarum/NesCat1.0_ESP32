#include "scene_holder.h"

#include "../log/log.h"

SceneHolder *__self = nullptr;
void (*__callback)(_u8 x, _u8 y, Color color) = nullptr;
void __onEachDurtyLine(DRTLine_t *line, _u8 lineNumber);

SceneHolder::SceneHolder(Palette_t *palette,
                         void (*onPixelUpdatedCallback)(_u8 x, _u8 y,
                                                        Color color)) {
  this->palette = palette;
  this->tracker = DRTrackerCreate();
  this->spritesHolder = SpritesHolderCreate();
  this->objectsHolder = ObjectsHolderCreate();

  __callback = onPixelUpdatedCallback;
  __self = this;
}

SceneHolder::~SceneHolder() {
  SpritesHolderDestroy(this->spritesHolder);
  ObjectsHolderDestroy(this->objectsHolder);
  DRTrackerDestroy(tracker);
}

ObjectId SceneHolder::createSprite(const SpriteData_t *data, _u8 x, _u8 y) {
  ObjectId id = SpritesHolderAddSprite(this->spritesHolder, data,
                                       SPRITE_TYPE_FOREGROUND, x, y);
  Sprite_t *sprite = (Sprite_t *)id;
  Rectangle_t *bounds = SpriteGetBounds(sprite);
  setDurtyRegion(bounds);
  return id;
}

ObjectId SceneHolder::createAnimatedSprite(
    const SpriteData_t *data, const AnimationSpeed_t animationSpeed,
    const _u8 x, const _u8 y) {
  ObjectId id = SpritesHolderAddAnimatedSprite(
      this->spritesHolder, data, SPRITE_TYPE_FOREGROUND, animationSpeed, x, y);
  AnimatedSprite_t *sprite = (AnimatedSprite_t *)id;
  Rectangle_t *bounds = AnimatedSpriteGetBounds(sprite);
  setDurtyRegion(bounds);
  return id;
}

ObjectId SceneHolder::createBackgroundSprite(const SpriteData_t *data, _u8 x,
                                             _u8 y) {
  ObjectId id = SpritesHolderAddSprite(this->spritesHolder, data,
                                       SPRITE_TYPE_BACKGROUND, x, y);
  Sprite_t *sprite = (Sprite_t *)id;
  Rectangle_t *bounds = SpriteGetBounds(sprite);
  setDurtyRegion(bounds);
  return id;
}

ObjectId SceneHolder::createGameObject(const SpriteData_t *data,
                                       bool isCollidable, bool isObstacle,
                                       bool isGravitable) {
  ObjectId sid = this->createSprite(data, 0, 0);

  return ObjectsHolderAdd(this->objectsHolder, sid, true, isCollidable,
                          isObstacle, isGravitable);
}

ObjectId SceneHolder::createBackgroundGameObject(const SpriteData_t *data,
                                                 bool isCollidable,
                                                 bool isObstacle,
                                                 bool isGravitable) {
  ObjectId sid = this->createBackgroundSprite(data, 0, 0);

  return ObjectsHolderAdd(objectsHolder, sid, false, isCollidable, isObstacle,
                          isGravitable);
}

void SceneHolder::moveSpriteBy(ObjectId spriteId, _i8 x, _i8 y) {
  Sprite_t *sprite = (Sprite_t *)spriteId;
  Rectangle_t *bounds = SpriteGetBounds(sprite);

  // set old region as durty
  setDurtyRegion(bounds);
  RectangleMoveBy(bounds, x, y);
  // as well as new
  setDurtyRegion(bounds);
}

void SceneHolder::moveSpriteTo(ObjectId spriteId, _i8 x, _i8 y) {
  Sprite_t *sprite = (Sprite_t *)spriteId;
  Rectangle_t *bounds = SpriteGetBounds(sprite);
  // set old region as durty
  setDurtyRegion(bounds);
  RectangleMoveTo(bounds, x, y);
  // as well as new
  setDurtyRegion(bounds);
}

void SceneHolder::calculateNextPosition(GameObject_t *object, _i8 moveByX,
                                        _i8 moveByY) {
  _u8 nextX, nextY = 0;

  Sprite_t *sprite = (Sprite_t *)GameObjectGetSpriteId(object);

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
  if (obstacleId == OBJECT_ID_NA) {
    obstacleId = findObjectOnScreen(nextPositionForCorner2->x,
                                    nextPositionForCorner2->y);
  }
  return obstacleId;
}

void SceneHolder::moveGameObjectBy(ObjectId id, _i8 x, _i8 y) {
  GameObject_t *objectToMove = (GameObject_t *)id;
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
  if (anotherObjectId == OBJECT_ID_NA) {
    ObjectId spriteId = GameObjectGetSpriteId(objectToMove);

    this->moveSpriteBy(spriteId, x, y);
    return;
  }

  GameObject_t *anotherObject = (GameObject_t *)anotherObjectId;
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

void SceneHolder::moveGameObjectTo(ObjectId id, _i8 x, _i8 y) {
  GameObject_t *object = (GameObject_t *)id;

  if (object == nullptr) return;
  ObjectId spriteId = GameObjectGetSpriteId(object);
  this->moveSpriteTo(spriteId, x, y);
}

_ci SceneHolder::findPixelInGameObjects(_u8 x, _u8 y, _ci defaultColorIndex) {
  ObjectId oid = ObjectsHolderGetObject(this->objectsHolder, true, x, y);
  if (oid == OBJECT_ID_NA) {
    oid = ObjectsHolderGetObject(this->objectsHolder, false, x, y);
  }

  if (oid == OBJECT_ID_NA) return defaultColorIndex;

  Sprite_t *sprite = (Sprite_t *)GameObjectGetSpriteId((GameObject_t *)oid);
  return SpriteGetPixel(sprite, x, y, defaultColorIndex);
}

_ci SceneHolder::findPixelInSprites(_u8 x, _u8 y, _ci defaultColorIndex) {
  SpriteType_t type = SPRITE_TYPE_FOREGROUND;
  ObjectId id = SpritesHolderGetSprite(spritesHolder, type, false, x, y);

  if (id != OBJECT_ID_NA) {
    Sprite_t *sprite = (Sprite_t *)id;
    _ci ci = SpriteGetPixel(sprite, x, y, defaultColorIndex);
    if (PaletteIsColorVisible(palette, ci) == true) {
      return ci;
    }
  }
  return defaultColorIndex;
}

_ci SceneHolder::findPixelInAnimatedSprites(_u8 x, _u8 y,
                                            _ci defaultColorIndex) {
  SpriteType_t type = SPRITE_TYPE_FOREGROUND;
  ObjectId id = SpritesHolderGetSprite(spritesHolder, type, true, x, y);

  if (id != OBJECT_ID_NA) {
    AnimatedSprite_t *sprite = (AnimatedSprite_t *)id;
    _ci ci = AnimatedSpriteGetPixel(sprite, x, y, defaultColorIndex);
    if (ci != defaultColorIndex) {
      return ci;
    }
  }
  return defaultColorIndex;
}

_ci SceneHolder::findPixelInBackgroundSprites(_u8 x, _u8 y,
                                              _ci defaultColorIndex) {
  SpriteType_t type = SPRITE_TYPE_BACKGROUND;

  ObjectId id = SpritesHolderGetSprite(spritesHolder, type, false, x, y);
  if (id != OBJECT_ID_NA) {
    Sprite_t *sprite = (Sprite_t *)id;
    _ci ci = SpriteGetPixel(sprite, x, y, defaultColorIndex);
    if (ci != defaultColorIndex) {
      return ci;
    }
  }
  return defaultColorIndex;
}

ObjectId SceneHolder::findObjectOnScreen(_u8 x, _u8 y) {
  ObjectId id = ObjectsHolderGetObject(this->objectsHolder, true, x, y);
  if (id == OBJECT_ID_NA) {
    id = ObjectsHolderGetObject(this->objectsHolder, false, x, y);
  }
  return id;
}

Color SceneHolder::calculatePixel(_u8 x, _u8 y) {
  _ci backgroundColorIndex = PalettegetBackgroundColor(palette);

  // check game object's sprites
  _ci colorIndex = findPixelInGameObjects(x, y, backgroundColorIndex);
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

void __onEachDurtyLine(DRTLine_t *line, _u8 lineNumber) {
  for (_u8 x = 0; x < WIDTH_IN_V_PIXELS; ++x) {
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
  SpritesHolderForeachAnimatedSprite(spritesHolder, SPRITE_TYPE_BACKGROUND,
                                     __updateAnimationStateCallback);

  SpritesHolderForeachAnimatedSprite(spritesHolder, SPRITE_TYPE_FOREGROUND,
                                     __updateAnimationStateCallback);

  SpritesHolderForeachAnimatedSprite(spritesHolder, SPRITE_TYPE_HUD,
                                     __updateAnimationStateCallback);
}

void SceneHolder::setDurtyRegion(const Rectangle_t *region) {
  _u8 left = RectangleGetVisibleLeftPosition(region);
  _u8 top = RectangleGetVisibleTopPosition(region);
  _u8 right = RectangleGetVisibleRightPosition(region);
  _u8 bottom = RectangleGetVisibleBottomPosition(region);
  DRTrackerSetDurtyRegion(tracker, left, top, right, bottom);
}

void SceneHolder::drawDurtyRegion() {
#ifdef PORT_SDK
  DRTrackerPrintDebugInfo(tracker);
#endif
}