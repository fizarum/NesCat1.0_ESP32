#ifndef NES_H
#define NES_H

#include "cpu.h"
#include "mmc.h"
#include "ppu.h"

#define MAX_MEM_HANDLERS 32

typedef struct nes_s {
  // hardware things
  nes6502_context *cpu;
  nes6502_memread readhandler[MAX_MEM_HANDLERS];
  nes6502_memwrite writehandler[MAX_MEM_HANDLERS];

  ppu_t *ppu;
  apu_t *apu;
  mmc_t *mmc;
  rominfo_t *rominfo;

  bool fiq_occurred;
  uint8_t fiq_state;
  int fiq_cycles;

  int scanline;

  // Timing stuff
  float scanline_cycles;
} nes_t;

#endif  // NES_H