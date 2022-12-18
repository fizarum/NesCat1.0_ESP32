#include "filename.h"

FileName *getLast(FileName *first);

FileName *createFileName(const char *name, FileName *prev, FileName *next) {
  FileName *f = (FileName *)malloc(sizeof(FileName));
  f->name = name;
  f->next = next;
  f->prev = prev;
  return f;
}

FileName *deleteFileName(FileName *element) {
  if (element == nullptr) {
    return nullptr;
  }

  // one-item-list
  if (element->prev == nullptr) {
    free(element);
    return nullptr;
  }

  // if list has more than one element in list
  FileName *current = element->prev;
  free(element);
  current->next = nullptr;
  return current;
}

void cleanup(FileName *filename) {
  if (filename == nullptr) {
    return;
  }
  FileName *current = nullptr;

  // find last element in list
  FileName *last = getLast(filename);

  last = deleteFileName(last);
  while (last != nullptr) {
    last = deleteFileName(last);
  }
}

FileName *getLast(FileName *first) {
  if (first == nullptr) {
    return nullptr;
  }
  FileName *current = first->next;

  while (current->next != nullptr) {
    current = current->next;
  }
  return current;
}

uint16_t getSize(FileName *first) {
  if (first == nullptr) {
    return 0;
  }

  FileName *current = first;
  uint16_t count = 1;
  while (current->next != nullptr) {
    current = current->next;
    count++;
    if (count == UINT16_MAX) {
      break;
    }
  }
  return count;
}

FileName *seek(FileName *first, uint16_t position) {
  if (first == nullptr) {
    return nullptr;
  }

  if (position == 0) {
    return first;
  }

  uint16_t counter = 0;

  FileName *current = first;
  while (current != nullptr) {
    current = current->next;
    counter++;
    if (counter == position) {
      return current;
    }
  }
  return nullptr;
}