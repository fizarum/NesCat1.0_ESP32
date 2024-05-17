#include "array.h"

#include <stdlib.h>
#include <string.h>

typedef struct Array_t {
  _ptr *values;
  // count of added items in array
  _u16 size;
  // total capacity
  _u16 capacity;
} Array_t;

Array_t *ArrayCreate(_u16 capcity) {
  Array_t *array = (Array_t *)malloc(sizeof(Array_t));
  array->capacity = capcity;
  array->size = 0;
  array->values = (_ptr *)calloc(capcity, sizeof(_ptr));
  return array;
}

void ArrayDestroy(Array_t *array) {
  if (array == NULL) return;

  free(array->values);
  free(array);
}

void ArrayClear(Array_t *array) {
  memset(array->values, 0, sizeof(_ptr) * (array->capacity));
  array->size = 0;
}

_u16 ArraySize(const Array_t *array) { return array->size; }
_u16 ArrayCapacity(const Array_t *array) { return array->capacity; }

bool ArrayAdd(Array_t *array, void *value) {
  if (ArrayIsFull(array) == true) return false;
  array->values[array->size] = (_ptr)value;
  array->size++;
}

bool ArrayRemove(Array_t *array, void *value) {
  _u16 indexToRemove = ArrayIndexOf(array, value);
  if (indexToRemove == ARRAY_INDEX_NONE) return false;

  array->values[indexToRemove] = 0;
  array->size--;
  for (_u16 index = indexToRemove; index < array->capacity; index++) {
    array->values[index] = array->values[index + 1];
  }
  return true;
}

void *ArrayValueAt(const Array_t *array, const _u16 index) {
  if (index != ARRAY_INDEX_NONE) {
    return (void *)array->values[index];
  }
  return NULL;
}

_u16 ArrayIndexOf(const Array_t *array, void *value) {
  for (_u16 index = 0; index < array->size; index++) {
    if (array->values[index] == (_ptr)value) {
      return index;
    }
  }
  return ARRAY_INDEX_NONE;
}

bool ArrayIsFull(const Array_t *array) {
  return array->size == array->capacity;
}

void ArrayForeach(const Array_t *array, void(callback)(void *value)) {
  for (_u16 index = 0; index < array->size; index++) {
    void *value = (void *)(array->values[index]);
    callback(value);
  }
}