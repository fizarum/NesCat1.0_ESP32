#include "scene_holder.h"

void setupDefaultPalette(Palette *palette);

SceneHolder *__self = nullptr;
void (*__callback)(uint8_t x, uint8_t y, Color color) = nullptr;
void __onEachDurtyPixel(uint8_t x, uint8_t y);

SceneHolder::SceneHolder(void (*onPixelUpdatedCallback)(uint8_t x, uint8_t y,
                                                        Color color)) {
  this->palette = new Palette();
  this->tracker = new DurtyRegionTracker();

  setupDefaultPalette(palette);
  __callback = onPixelUpdatedCallback;
  __self = this;
}

SceneHolder::~SceneHolder() {
  for (auto &it : spritesWithId) {
    delete it.second;
  }
  spritesWithId.clear();
  delete tracker;
  delete palette;
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
  _lastAssignedIdForSprite++;
  Sprite *sprite = createPlainSprite(width, height, pixels, pixelsCount,
                                     positionX, positionY);
  spritesWithId[_lastAssignedIdForSprite] = sprite;

  return _lastAssignedIdForSprite;
}

ObjectId SceneHolder::createBackgroundSprite(uint8_t width, uint8_t height,
                                             ColorIndex pixels[],
                                             size_t pixelsCount,
                                             uint8_t positionX,
                                             uint8_t positionY) {
  _lastAssignedIdForBackgroundSprite++;
  Sprite *sprite = createPlainSprite(width, height, pixels, pixelsCount,
                                     positionX, positionY);
  backgroundSpritesWithId[_lastAssignedIdForBackgroundSprite] = sprite;

  return _lastAssignedIdForBackgroundSprite;
}

ObjectId SceneHolder::createGameObject(uint8_t width, uint8_t height,
                                       ColorIndex pixels[], size_t pixelsCount,
                                       bool isCollidable, bool isGravitable) {
  _lastAssignedIdForSprite++;
  Sprite *sprite = createPlainSprite(width, height, pixels, pixelsCount, 0, 0);
  spritesWithId[_lastAssignedIdForSprite] = sprite;

  _lastAssignedIdForGameObject++;
  GameObject *gameObject =
      new GameObject(_lastAssignedIdForSprite, true, false);
  gameObjectsWithId[_lastAssignedIdForGameObject] = gameObject;

  return _lastAssignedIdForGameObject;
}

void SceneHolder::moveSpriteBy(ObjectId spriteId, int8_t x, int8_t y) {
  Sprite *sprite = spritesWithId[spriteId];
  // set old region as durty
  setDurtyRegion(sprite, 1);
  sprite->moveBy(x, y);
  // as well as new
  setDurtyRegion(sprite);
}

void SceneHolder::moveGameObjectBy(ObjectId id, int8_t x, int8_t y) {
  GameObject *gameObject = gameObjectsWithId[id];
  if (gameObject == nullptr) return;
  // todo: add collision logic

  ObjectId spriteId = gameObject->getSpriteId();
  this->moveSpriteBy(spriteId, x, y);
}

ColorIndex SceneHolder::findPixelInGameObjects(uint8_t x, uint8_t y) {
  ObjectId spriteId;
  ColorIndex ci = COLOR_INDEX_UNDEF;

  for (auto &it : gameObjectsWithId) {
    spriteId = it.second->getSpriteId();
    Sprite *sprite = spritesWithId[spriteId];
    if (sprite != nullptr) {
      ci = sprite->getPixel(x, y);
      if (ci != COLOR_INDEX_UNDEF && ci != COLOR_INDEX_TRANSPARENT) {
        return ci;
      }
    }
  }
  return COLOR_INDEX_UNDEF;
}

ColorIndex SceneHolder::findPixelInSprites(uint8_t x, uint8_t y) {
  for (auto &it : spritesWithId) {
    ColorIndex ci = it.second->getPixel(x, y);
    // second check allows to continue searching in siblin sprites
    // when current one has transparent pixel
    if (ci != COLOR_INDEX_UNDEF && ci != COLOR_INDEX_TRANSPARENT) {
      return ci;
    }
  }
  return COLOR_INDEX_UNDEF;
}

ColorIndex SceneHolder::findPixelInBackgroundSprites(uint8_t x, uint8_t y) {
  for (auto &it : backgroundSpritesWithId) {
    ColorIndex ci = it.second->getPixel(x, y);
    if (ci != COLOR_INDEX_UNDEF) {
      return ci;
    }
  }
  return COLOR_INDEX_UNDEF;
}

Color SceneHolder::calculatePixel(uint8_t x, uint8_t y) {
  // check game object's sprites
  ColorIndex colorIndex = findPixelInGameObjects(x, y);
  if (ifColorIndexHasColor(colorIndex) == true) {
    return palette->getColor(colorIndex);
  }

  // else foreground sprites
  colorIndex = findPixelInSprites(x, y);
  if (ifColorIndexHasColor(colorIndex) == true) {
    return palette->getColor(colorIndex);
  }

  // nothing found? take background once
  colorIndex = findPixelInBackgroundSprites(x, y);
  if (ifColorIndexHasColor(colorIndex) == true) {
    // if pixel not taken even by background sprite, take def value
    return palette->getColor(colorIndex);
  }

  // if pixel not taken even by background sprite, take def value
  return palette->getColor(COLOR_INDEX_BACKGROUND);
}

void __onEachDurtyPixel(uint8_t x, uint8_t y) {
  Color color = __self->calculatePixel(x, y);
  if (__callback != nullptr) {
    __callback(x, y, color);
  }
}

void SceneHolder::bakeCanvas() {
  tracker->foreachDurtyPixel(&__onEachDurtyPixel);
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

void SceneHolder::drawDurtyRegion() { tracker->printDebugInfo(); }

void setupDefaultPalette(Palette *palette) {
  palette->setColor(0, COLOR_BLACK);
  palette->setColor(1, COLOR_PEARL);
  palette->setColor(2, COLOR_WATERMELON_RED);
  palette->setColor(3, COLOR_PEWTER_BLUE);
  palette->setColor(4, COLOR_PURPLE_TAUPE);
  palette->setColor(5, COLOR_FOREST_GREEN);
  palette->setColor(6, COLOR_INDIGO);
  palette->setColor(7, COLOR_SUNRAY);
  palette->setColor(8, COLOR_LIGHT_TAUPE);
  palette->setColor(9, COLOR_FELDGRAU);
  palette->setColor(10, COLOR_CEDAR_CHEST);
  palette->setColor(11, COLOR_DARK_CHARCOAL);
  palette->setColor(12, COLOR_SONIC_SILVER);
  palette->setColor(13, COLOR_ASPARAGUS);
  palette->setColor(14, COLOR_SEA_SERPENT);
  palette->setColor(15, COLOR_GRAY);
  //
  palette->setColor(COLOR_INDEX_BACKGROUND, COLOR_BLACK);
}