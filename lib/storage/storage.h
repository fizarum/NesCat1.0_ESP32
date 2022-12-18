#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdlib.h>

void storageInit();
/** get filenames in specified path */
void getFilenames(const char* path, void (*callback)(const char*),
                  const char* extension = nullptr);

/** scans filename for provided extension
 * @param filename to scan for extension
 * @param extension with length smaller than 10 symbols
 * */
bool hasSuchExtension(const char* filename, const char* extension);

size_t getFileSize(const char* filepath);

/**
 * @brief read file's content
 *
 *
 * @param filepath - name of file to read
 * @param callback - has read buff, and count of read bytes, buff index and
 * bool flag if this portion is last
 * @param start position from which reading should start
 * @return bool is file read successfully
 */
bool readFile(const char* filepath,
              void (*callback)(uint8_t*, uint16_t, uint16_t, bool),
              uint32_t start = 0);

/** erase spi flash aligned to 4kb*/
bool eraseFlash(size_t fileSize);

bool savePortionToFlash(uint8_t* block, uint16_t blockLength,
                        uint16_t blockIndex);
uint8_t* getSavedDataOnFlash(size_t romSize);

#endif  // STORAGE_H