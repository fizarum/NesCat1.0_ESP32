#ifndef nes_wrapper_h
#define nes_wrapper_h

#include "filename.h"
#include "nes/nes.h"

nes_t *createNes();

FileName *getAllNesFiles(const char *path);

void pickRomFile(const char *filepath);

/** callback(percents, isFinished) has percents as an argument and bool flag
 * indicating is operation finished*/
bool getRomData(const char *filepath, void (*callback)(uint8_t, bool));

/** untested */
void preparePsRam();

#endif  // nes_wrapper_h