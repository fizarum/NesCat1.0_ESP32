#ifndef ROM_H
#define ROM_H

#include <stdint.h>

typedef enum { MIRROR_HORIZ = 0, MIRROR_VERT = 1 } mirror_t;

#define ROM_FLAG_BATTERY 0x01
#define ROM_FLAG_TRAINER 0x02
#define ROM_FLAG_FOURSCREEN 0x04
#define ROM_FLAG_VERSUS 0x08

typedef struct rominfo_s {
  // pointers to ROM and VROM
  uint8_t *rom, *vrom;

  // pointers to SRAM and VRAM
  uint8_t *sram, *vram;

  // number of banks
  int rom_banks, vrom_banks;
  int sram_banks, vram_banks;

  int mapper_number;
  mirror_t mirror;

  uint8_t flags;
} rominfo_t;

typedef struct inesheader_s {
  uint8_t ines_magic[4];
  uint8_t rom_banks;
  uint8_t vrom_banks;
  uint8_t rom_type;
  uint8_t mapper_hinybble;
  uint8_t reserved[8];
} inesheader_t;

int rom_allocsram(rominfo_t *rominfo);
void rom_loadtrainer(unsigned char **rom, rominfo_t *rominfo);
int rom_loadrom(unsigned char **rom, rominfo_t *rominfo);
int rom_getheader(unsigned char **rom, rominfo_t *rominfo);
void rom_free(rominfo_t **rominfo);

#endif  // ROM_H
