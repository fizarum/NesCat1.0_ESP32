#ifndef GAME_SDK_GAME_OBJECT_H
#define GAME_SDK_GAME_OBJECT_H

#include "point.h"
#include "sprite.h"

class GameObject {
 private:
  ObjectId spriteId;
  /**
   * @brief Indicates if collision detection should be triggered for this object
   */
  bool collidable;
  /**
   * @brief if set to true another object can't move through this one
   */
  bool obstacle;

  /**
   * @brief if set to true object will be added to gravity force calculations
   */
  bool gravitable;

  /**
   * @brief precalculated position of object's corners during movement
   * for movement by diagonal only one corner is updated, but when object moves
   * by horizontal/vertical axis 2 corners should be updated
   */
  Point nextPositionOfCorner1;
  Point nextPositionOfCorner2;

 public:
  GameObject(ObjectId spriteId, bool collidable = false, bool obstacle = false,
             bool gravitable = false) {
    this->spriteId = spriteId;
    this->collidable = collidable;
    this->obstacle = obstacle;
    this->gravitable = gravitable;
  }

  ObjectId getSpriteId() { return spriteId; }

  inline bool isCollidable() { return this->collidable; }
  inline bool isObstacle() { return this->obstacle; }

  Point *getNextPositionOfCorner1() { return &nextPositionOfCorner1; }
  Point *getNextPositionOfCorner2() { return &nextPositionOfCorner2; }

  void setNextPositionForCorner1(int16_t x, int16_t y) {
    this->nextPositionOfCorner1.set(x, y);
  }
  void setNextPositionForCorner2(int16_t x, int16_t y) {
    this->nextPositionOfCorner2.set(x, y);
  }

  void resetNextPositionOfCorner1() { this->nextPositionOfCorner1.reset(); }
  void resetNextPositionOfCorner2() { this->nextPositionOfCorner2.reset(); }
};

#endif  // GAME_SDK_GAME_OBJECT_H