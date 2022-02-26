#ifndef MMC_H
#define MMC_H

#include <stdint.h>

#include "apu.h"
#include "libsns.h"
#include "rom.h"

#define MMC_LASTBANK -1

typedef struct {
  uint32_t min_range, max_range;
  uint8_t (*read_func)(uint32_t address);
} map_memread;

typedef struct {
  uint32_t min_range, max_range;
  void (*write_func)(uint32_t address, uint8_t value);
} map_memwrite;

typedef struct mapintf_s {
  int number;
  const char *name;
  void (*init)(void);
  void (*vblank)(void);
  void (*hblank)(int vblank);
  void (*get_state)(SnssMapperBlock *state);
  void (*set_state)(SnssMapperBlock *state);
  map_memread *mem_read;
  map_memwrite *mem_write;
  apuext_t *sound_ext;
} mapintf_t;

typedef struct mmc_s {
  mapintf_t *intf;
  rominfo_t *cart;  // link it back to the cart
} mmc_t;

#endif  // MMC_H