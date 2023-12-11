#ifndef GAME_SDK_CONTROLLER_H
#define GAME_SDK_CONTROLLER_H

#include <map>
#include <vector>

#include "durty_region_tracker.h"
#include "rectangle.h"
#include "sprite.h"

typedef uint16_t SpriteId;

class SceneHolder {
 private:
  std::map<SpriteId, Sprite *> spritesWithId = {};
  std::map<SpriteId, Sprite *> backgroundSpritesWithId = {};
  Palette *palette;
  DurtyRegionTracker *tracker;
  SpriteId _lastAssignedIdForSprite = 0;
  SpriteId _lastAssignedIdForBackgroundSprite = 0;
  Sprite *createPlainSprite(uint8_t width, uint8_t height, ColorIndex pixels[],
                            size_t pixelsCount, uint8_t positionX,
                            uint8_t positionY);
  ColorIndex findPixelInSprites(uint8_t x, uint8_t y);
  ColorIndex findPixelInBackgroundSprites(uint8_t x, uint8_t y);

 public:
  SceneHolder(void (*onPixelUpdatedCallback)(uint8_t x, uint8_t y,
                                             Color color));
  ~SceneHolder();

  SpriteId createSprite(uint8_t width, uint8_t height, ColorIndex pixels[],
                        size_t pixelsCount, uint8_t positionX = 0,
                        uint8_t positionY = 0);
  SpriteId createBackgroundSprite(uint8_t width, uint8_t height,
                                  ColorIndex pixels[], size_t pixelsCount,
                                  uint8_t positionX = 0, uint8_t positionY = 0);

  void moveSpriteBy(SpriteId spriteId, int8_t x, int8_t y);

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
};

#endif  // GAME_SDK_CONTROLLER_H