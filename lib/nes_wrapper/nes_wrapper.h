#ifndef nes_wrapper_h
#define nes_wrapper_h

#include "filename.h"
#include "nes/nes.h"

nes_t *createNes();

FileName *getAllNesFiles(const char *path);

#endif  // nes_wrapper_h