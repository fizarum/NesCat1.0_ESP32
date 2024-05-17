#include "objects_holder.h"

#include <stdlib.h>

#include "../collections/array.h"
#include "../scene/game_object.h"
#include "sprites_holder.h"

// max objects count is 32
#define OBJECTS_BACKGROUND_MAX_COUNT 12
#define OBJECTS_FOREGROUND_MAX_COUNT 20

typedef struct ObjectsHolder_t {
  Array_t *backgroundObjects;
  Array_t *foregroundObjects;
} ObjectsHolder_t;

inline void destroyGameObject(void *gameObject);

Array_t *selectObjectsContainer(const ObjectsHolder_t *holder, bool foreground);

ObjectsHolder_t *ObjectsHolderCreate() {
  ObjectsHolder_t *holder = (ObjectsHolder_t *)malloc(sizeof(ObjectsHolder_t));
  holder->backgroundObjects = ArrayCreate(OBJECTS_BACKGROUND_MAX_COUNT);
  holder->foregroundObjects = ArrayCreate(OBJECTS_FOREGROUND_MAX_COUNT);

  return holder;
}

void ObjectsHolderDestroy(ObjectsHolder_t *holder) {
  if (holder == NULL) return;

  ArrayForeach(holder->backgroundObjects, destroyGameObject);
  ArrayForeach(holder->foregroundObjects, destroyGameObject);

  ArrayDestroy(holder->backgroundObjects);
  ArrayDestroy(holder->foregroundObjects);

  free(holder);
}

ObjectId ObjectsHolderAdd(ObjectsHolder_t *holder, ObjectId spriteId,
                          bool foreground, bool collidable, bool obstacle,
                          bool gravitable) {
  GameObject_t *object =
      GameObjectCreate(spriteId, collidable, obstacle, gravitable);
  Array_t *container = selectObjectsContainer(holder, foreground);
  bool added = ArrayAdd(container, object);
  if (added == true) {
    return (ObjectId)object;
  }
  return OBJECT_ID_NA;
}

ObjectId ObjectsHolderGetObject(const ObjectsHolder_t *holder,
                                const bool foreground, const _u8 x,
                                const _u8 y) {
  Array_t *container = selectObjectsContainer(holder, foreground);
  _u16 size = ArraySize(container);

  for (_u16 index = 0; index < size; index++) {
    GameObject_t *object = ArrayValueAt(container, index);
    Sprite_t *sprite = (Sprite_t *)GameObjectGetSpriteId(object);
    Rectangle_t *bounds = SpriteGetBounds(sprite);
    if (RectangleContainsPoint(bounds, x, y)) {
      return (ObjectId)object;
    }
  }
  return OBJECT_ID_NA;
}

Array_t *selectObjectsContainer(const ObjectsHolder_t *holder,
                                bool foreground) {
  if (foreground == true) {
    return holder->foregroundObjects;
  } else {
    return holder->backgroundObjects;
  }
}

void destroyGameObject(void *gameObject) {
  GameObjectDestroy((GameObject_t *)gameObject);
}