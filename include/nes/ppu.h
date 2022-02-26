#ifndef PPU_H
#define PPU_H

#include <stdint.h>

// PPU register defines
#define PPU_CTRL0 0x2000
#define PPU_CTRL1 0x2001
#define PPU_STAT 0x2002
#define PPU_OAMADDR 0x2003
#define PPU_OAMDATA 0x2004
#define PPU_SCROLL 0x2005
#define PPU_VADDR 0x2006
#define PPU_VDATA 0x2007

#define PPU_OAMDMA 0x4014
#define PPU_JOY0 0x4016
#define PPU_JOY1 0x4017

// $2000
#define PPU_CTRL0F_NMI 0x80
#define PPU_CTRL0F_OBJ16 0x20
#define PPU_CTRL0F_BGADDR 0x10
#define PPU_CTRL0F_OBJADDR 0x08
#define PPU_CTRL0F_ADDRINC 0x04
#define PPU_CTRL0F_NAMETAB 0x03

// $2001
#define PPU_CTRL1F_OBJON 0x10
#define PPU_CTRL1F_BGON 0x08
#define PPU_CTRL1F_OBJMASK 0x04
#define PPU_CTRL1F_BGMASK 0x02

// $2002
#define PPU_STATF_VBLANK 0x80
#define PPU_STATF_STRIKE 0x40
#define PPU_STATF_MAXSPRITE 0x20

// Sprite attribute byte bitmasks
#define OAMF_VFLIP 0x80
#define OAMF_HFLIP 0x40
#define OAMF_BEHIND 0x20

// Maximum number of sprites per horizontal scanline
#define PPU_MAXSPRITE 8

// some mappers do *dumb* things
typedef void (*ppulatchfunc_t)(uint32_t address, uint8_t value);
typedef void (*ppuvromswitch_t)(uint8_t value);

typedef struct ppu_s {
  // big nasty memory chunks
  uint8_t nametab[0x1000];
  uint8_t oam[256];
  uint8_t palette[32];
  uint8_t *page[16];

  // hardware registers
  uint8_t ctrl0, ctrl1, stat, oam_addr;
  uint32_t vaddr, vaddr_latch;
  int tile_xofs, flipflop;
  int vaddr_inc;
  uint32_t tile_nametab;

  uint8_t obj_height;
  uint32_t obj_base, bg_base;

  bool bg_on, obj_on;
  bool obj_mask, bg_mask;

  uint8_t latch, vdata_latch;
  uint8_t strobe;

  bool strikeflag;
  uint32_t strike_cycle;

  // callbacks for naughty mappers
  ppulatchfunc_t latchfunc;
  ppuvromswitch_t vromswitch;

  bool vram_accessible;

  bool vram_present;
  bool drawsprites;
} ppu_t;

#endif  // PPU_H