#ifndef ARRAY_H
#define ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "../types/simpler_types.h"

#define ARRAY_INDEX_NONE UINT16_MAX

typedef struct Array_t Array_t;

Array_t *ArrayCreate(_u16 capcity);
void ArrayDestroy(Array_t *array);
void ArrayClear(Array_t *array);

_u16 ArraySize(Array_t *array);
_u16 ArrayCapacity(Array_t *array);

bool ArrayAdd(Array_t *array, void *value);
bool ArrayRemove(Array_t *array, void *value);

void *ArrayValueOf(Array_t *array, _u16 index);
_u16 ArrayIndexOf(Array_t *array, void *value);

bool ArrayIsFull(Array_t *array);

#ifdef __cplusplus
}
#endif

#endif  // ARRAY_H