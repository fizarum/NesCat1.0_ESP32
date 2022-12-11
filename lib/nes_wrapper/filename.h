#ifndef filename_h
#define filename_h

#include <cstdlib>

/** simple structure for representing file names on FS */
struct FileName {
  const char *name;
  FileName *prev;
  FileName *next;
};

typedef struct FileName FileName;

FileName *createFileName(const char *name = nullptr, FileName *prev = nullptr,
                         FileName *next = nullptr);

/** free resources taken by element and
 * return a pointer to previous element in
 * list, of nullptr if its one-item-list */
FileName *deleteFileName(FileName *element);

/** remove all item in list */
void cleanup(FileName *filename);

/** get last element in linked list */
FileName *getLast(FileName *first);

/** get size of list */
uint16_t getSize(FileName *first);

/** seek to specified position in list */
FileName *seek(FileName *first, uint16_t position = 0);

#endif  // filename_h