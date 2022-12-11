#ifndef NES_H
#define NES_H

#include "cpu.h"
#include "mmc.h"
#include "ppu.h"

#define MAX_MEM_HANDLERS 32

#define NES_CLOCK_DIVIDER 12  // default: 12
// #define  NES_MASTER_CLOCK     21477272.727272727272
#define NES_MASTER_CLOCK (236250000 / 11)
#define NES_SCANLINE_CYCLES (1364.0 / NES_CLOCK_DIVIDER)
#define NES_FIQ_PERIOD (NES_MASTER_CLOCK / NES_CLOCK_DIVIDER / 60)

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