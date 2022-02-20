#ifndef ROMLOADER_H
#define ROMLOADER_H

extern uint8_t *PSRAM;
extern uint32_t psRAMSize;

// void loadToPsRam(SdFat *sd, uint8_t *psRam);

uint8_t *getromdata(SdFat sd, const char *filename);

#endif  // ROMLOADER_H