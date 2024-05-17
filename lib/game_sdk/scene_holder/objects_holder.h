#ifndef GAME_SDK_OBJECTS_HOLDER_H
#define GAME_SDK_OBJECTS_HOLDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../primitives/primitives.h"

typedef struct ObjectsHolder_t ObjectsHolder_t;

ObjectsHolder_t *ObjectsHolderCreate();
void ObjectsHolderDestroy(ObjectsHolder_t *holder);

ObjectId ObjectsHolderAdd(ObjectsHolder_t *holder, ObjectId spriteId,
                          bool foreground, bool collidable, bool obstacle,
                          bool gravitable);

ObjectId ObjectsHolderGetObject(const ObjectsHolder_t *holder,
                                const bool foreground, const _u8 x,
                                const _u8 y);
#ifdef __cplusplus
}
#endif
#endif  // GAME_SDK_OBJECTS_HOLDER_H