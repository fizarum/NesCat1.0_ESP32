#ifndef STORAGE_H
#define STORAGE_H

void storageInit();
/** get filenames in specified path */
void getFilenames(const char* path, void (*callback)(const char*),
                  const char* extension = nullptr);

/** scans filename for provided extension
 * @param filename to scan for extension
 * @param extension with length smaller than 10 symbols
 * */
bool hasSuchExtension(const char* filename, const char* extension);

#endif  // STORAGE_H