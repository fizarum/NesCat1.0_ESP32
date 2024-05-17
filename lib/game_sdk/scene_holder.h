#ifndef GAME_SDK_CONTROLLER_H
#define GAME_SDK_CONTROLLER_H

#include "../types/simpler_types.h"
#include "durty_region_tracker/tracker.h"
#include "palette.h"
#include "scene/animated_sprite.h"
#include "scene/game_object.h"
#include "scene/sprite.h"
#include "scene_holder/objects_holder.h"
#include "scene_holder/sprites_holder.h"

class SceneHolder {
 private:
  Palette_t *palette;
  DRTracker_t *tracker;
  SpritesHolder_t *spritesHolder;
  ObjectsHolder_t *objectsHolder;

  _ci findPixelInGameObjects(_u8 x, _u8 y, _ci defaultColorIndex);
  _ci findPixelInSprites(_u8 x, _u8 y, _ci defaultColorIndex);
  _ci findPixelInAnimatedSprites(_u8 x, _u8 y, _ci defaultColorIndex);
  _ci findPixelInBackgroundSprites(_u8 x, _u8 y, _ci defaultColorIndex);

  /**
   * @brief Find any object which has given pixel
   *
   * @param x - x coord of pixel
   * @param y - y coord of pixel
   * @return ObjectId of found object, or ObjectIdUndef
   */
  ObjectId findObjectOnScreen(_u8 x, _u8 y);

  /**
   * @brief calculates next position of point closest to any possible obstacle
   *
   * @param geometry - original object to move
   * @param moveByX - value on which original object moves by horizontal
   * @param moveByY - value on which original object moves by vertical
   */
  void calculateNextPosition(GameObject_t *object, _i8 moveByX, _i8 moveByY);

  /**
   * @brief Get the obstacle for moving object
   *
   * @param object - gameObject which performs movement
   * @return ObjectId for any obstacle if present or OBJECT_ID_NA otherwise
   */
  ObjectId getObstacle(GameObject_t *object);

 public:
  SceneHolder(Palette_t *palette,
              void (*onPixelUpdatedCallback)(_u8 x, _u8 y, Color color));
  ~SceneHolder();

  ObjectId createSprite(_u8 width, _u8 height, _ci pixels[], size_t pixelsCount,
                        _u8 x = 0, _u8 y = 0);

  ObjectId createAnimatedSprite(_u8 width, _u8 height, _ci *const pixels,
                                size_t pixelsCount,
                                const AnimationSpeed_t animationSpeed,
                                const _u8 x, const _u8 y);

  ObjectId createBackgroundSprite(_u8 width, _u8 height, _ci *const pixels,
                                  size_t pixelsCount, _u8 x = 0, _u8 y = 0);

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
  ObjectId createGameObject(_u8 width, _u8 height, _ci pixels[],
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
  ObjectId createBackgroundGameObject(_u8 width, _u8 height, _ci pixels[],
                                      size_t pixelsCount,
                                      bool isCollidable = false,
                                      bool isObstacle = false,
                                      bool isGravitable = false);

  void moveSpriteBy(ObjectId spriteId, _i8 x, _i8 y);
  void moveSpriteTo(ObjectId spriteId, _i8 x, _i8 y);
  void moveGameObjectBy(ObjectId id, _i8 x, _i8 y);
  void moveGameObjectTo(ObjectId id, _i8 x, _i8 y);

  void bakeCanvas();

  void updateAnimationState();

  Color calculatePixel(_u8 x, _u8 y);

  /**
   * @brief Set the Durty Region which will be redrawn in next draw call
   *
   * @param region which should be redrawn
   */
  void setDurtyRegion(const Rectangle_t *region);

  void drawDurtyRegion();
};

#endif  // GAME_SDK_CONTROLLER_H