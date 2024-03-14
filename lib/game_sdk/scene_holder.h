#ifndef GAME_SDK_CONTROLLER_H
#define GAME_SDK_CONTROLLER_H

#include <map>
#include <vector>

#include "durty_region_tracker.h"
#include "game_object.h"
#include "rectangle.h"
#include "sprite.h"

class SceneHolder {
 private:
  std::map<ObjectId, Sprite *> spritesWithId = {};
  std::map<ObjectId, Sprite *> backgroundSpritesWithId = {};
  std::map<ObjectId, GameObject *> gameObjectsWithId = {};

  Palette *palette;
  DurtyRegionTracker *tracker;
  ObjectId _lastAssignedIdForSprite = 0;
  ObjectId _lastAssignedIdForBackgroundSprite = 0;
  ObjectId _lastAssignedIdForGameObject = 0;

  Sprite *createPlainSprite(uint8_t width, uint8_t height, ColorIndex pixels[],
                            size_t pixelsCount, uint8_t positionX,
                            uint8_t positionY);

  ColorIndex findPixelInGameObjects(uint8_t x, uint8_t y);
  ColorIndex findPixelInSprites(uint8_t x, uint8_t y);
  ColorIndex findPixelInBackgroundSprites(uint8_t x, uint8_t y);

  inline Sprite *getSprite(GameObject *object) {
    if (object == nullptr) return nullptr;
    ObjectId spriteId = object->getSpriteId();
    return spritesWithId[spriteId];
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
  SceneHolder(void (*onPixelUpdatedCallback)(uint8_t x, uint8_t y,
                                             Color color));
  ~SceneHolder();

  ObjectId createSprite(uint8_t width, uint8_t height, ColorIndex pixels[],
                        size_t pixelsCount, uint8_t positionX = 0,
                        uint8_t positionY = 0);
  ObjectId createBackgroundSprite(uint8_t width, uint8_t height,
                                  ColorIndex pixels[], size_t pixelsCount,
                                  uint8_t positionX = 0, uint8_t positionY = 0);

  /**
   * @brief Create a Game Object object
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
                            bool isGravitable = false);

  void moveSpriteBy(ObjectId spriteId, int8_t x, int8_t y);
  void moveGameObjectBy(ObjectId id, int8_t x, int8_t y);

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