#ifndef GAME_SDK_CONTROLLER_H
#define GAME_SDK_CONTROLLER_H

#include <map>
#include <vector>

#include "durty_region_tracker/tracker.h"
#include "game_object.h"
#include "rectangle.h"
#include "sprite.h"

typedef std::map<ObjectId, Sprite *>::iterator SpriteIterator;
typedef std::map<ObjectId, GameObject *>::iterator GameObjectIterator;

class SceneHolder {
 private:
  std::map<ObjectId, Sprite *> spritesWithId = {};
  std::map<ObjectId, Sprite *> backgroundSpritesWithId = {};
  std::map<ObjectId, GameObject *> backgroundGameObjectsWithId = {};
  std::map<ObjectId, GameObject *> gameObjectsWithId = {};

  Palette_t *palette;
  DurtyRegionTracker *tracker;
  ObjectId lastAssignedId = 0;

  Sprite *createPlainSprite(uint8_t width, uint8_t height, ColorIndex pixels[],
                            size_t pixelsCount, uint8_t positionX,
                            uint8_t positionY);

  ColorIndex findPixelInGameObjects(uint8_t x, uint8_t y,
                                    ColorIndex defaultColorIndex);
  ColorIndex findPixelInSprites(uint8_t x, uint8_t y,
                                ColorIndex defaultColorIndex);
  ColorIndex findPixelInBackgroundSprites(uint8_t x, uint8_t y,
                                          ColorIndex defaultColorIndex);

  inline Sprite *getSprite(ObjectId id) {
    SpriteIterator it = spritesWithId.find(id);
    if (it != spritesWithId.end()) {
      return it->second;
    }

    it = backgroundSpritesWithId.find(id);
    if (it != backgroundSpritesWithId.end()) {
      return it->second;
    }

    return nullptr;
  }

  inline Sprite *getSprite(GameObject *object) {
    if (object == nullptr) return nullptr;

    return this->getSprite(object->getSpriteId());
  }

  inline GameObject *getGameObject(ObjectId id) {
    GameObjectIterator it = gameObjectsWithId.find(id);
    if (it != gameObjectsWithId.end()) {
      return it->second;
    }

    it = backgroundGameObjectsWithId.find(id);
    if (it != backgroundGameObjectsWithId.end()) {
      return it->second;
    }

    return nullptr;
  }

  /**
   * @brief Find any object which has given pixel
   *
   * @param x - x coord of pixel
   * @param y - y coord of pixel
   * @return ObjectId of found object, or ObjectIdUndef
   */
  ObjectId findObjectOnScreen(uint8_t x, uint8_t y);

  /**
   * @brief calculates next position of point closest to any possible obstacle
   *
   * @param geometry - original object to move
   * @param moveByX - value on which original object moves by horizontal
   * @param moveByY - value on which original object moves by vertical
   */
  void calculateNextPosition(GameObject *object, int8_t moveByX,
                             int8_t moveByY);

  /**
   * @brief Get the obstacle for moving object
   *
   * @param object - gameObject which performs movement
   * @return ObjectId for any obstacle if present or OBJECT_ID_UNDEF otherwise
   */
  ObjectId getObstacle(GameObject *object);

 public:
  SceneHolder(Palette_t *palette,
              void (*onPixelUpdatedCallback)(uint8_t x, uint8_t y,
                                             Color color));
  ~SceneHolder();

  ObjectId createSprite(uint8_t width, uint8_t height, ColorIndex pixels[],
                        size_t pixelsCount, uint8_t positionX = 0,
                        uint8_t positionY = 0);

  ObjectId createBackgroundSprite(uint8_t width, uint8_t height,
                                  ColorIndex pixels[], size_t pixelsCount,
                                  uint8_t positionX = 0, uint8_t positionY = 0);

  /**
   * @brief Create a Game Object for foreground plane
   *
   * @param width
   * @param height
   * @param pixels
   * @param pixelsCount
   * @param isCollidable
   * @param isGravitable
   * @return ObjectId
   */
  ObjectId createGameObject(uint8_t width, uint8_t height, ColorIndex pixels[],
                            size_t pixelsCount, bool isCollidable = false,
                            bool isObstacle = false, bool isGravitable = false);

  /**
   * @brief Create a Game Object for background plane
   *
   * @param width
   * @param height
   * @param pixels
   * @param pixelsCount
   * @param isCollidable
   * @param isGravitable
   * @return ObjectId
   */
  ObjectId createBackgroundGameObject(uint8_t width, uint8_t height,
                                      ColorIndex pixels[], size_t pixelsCount,
                                      bool isCollidable = false,
                                      bool isObstacle = false,
                                      bool isGravitable = false);

  void moveSpriteBy(ObjectId spriteId, int8_t x, int8_t y);
  void moveSpriteTo(ObjectId spriteId, int8_t x, int8_t y);
  void moveGameObjectBy(ObjectId id, int8_t x, int8_t y);
  void moveGameObjectTo(ObjectId id, int8_t x, int8_t y);

  void bakeCanvas();
  Color calculatePixel(uint8_t x, uint8_t y);

  void setDurtyRegion(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom);
  /**
   * @brief Set the Durty Region which will be redrawn in next draw call
   *
   * @param region which should be redrawn
   */
  void setDurtyRegion(Rectangle *region, uint8_t extraSpace = 0);
  void removeAllDurtyRegions();

  void drawDurtyRegion();
};

#endif  // GAME_SDK_CONTROLLER_H