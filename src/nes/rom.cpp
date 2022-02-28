#include "nes/rom.h"

#include <stdlib.h>
#include <string.h>
#include <utils.h>

#define SRAM_BANK_LENGTH 0x0400
#define TRAINER_OFFSET 0x1000
#define TRAINER_LENGTH 0x200

#define ROM_BANK_LENGTH 0x4000
#define VROM_BANK_LENGTH 0x2000
#define VRAM_LENGTH 0x2000

#define RESERVED_LENGTH 8

#define ROM_INES_MAGIC "NES\x1A"

#define ROM_MIRRORTYPE 0x01
#define ROM_BATTERY 0x02
#define ROM_TRAINER 0x04
#define ROM_FOURSCREEN 0x08

// Max length for displayed filename
// #define ROM_DISP_MAXLEN 20
// #define VRAM_BANK_LENGTH 0x2000
// uint8_t SRAM_allocated = 0;

// Allocate space for SRAM
int rom_allocsram(rominfo_t *rominfo) {
  // Load up SRAM
  if (NULL == rominfo->sram)
    rominfo->sram = (uint8_t *)malloc(SRAM_BANK_LENGTH * rominfo->sram_banks);
  if (NULL == rominfo->sram) {
    /// gui_sendmsg(GUI_RED, "Could not allocate space for battery RAM");
    return -1;
  }

  // make damn sure SRAM is clear
  memset(rominfo->sram, 0, SRAM_BANK_LENGTH * rominfo->sram_banks);
  return 0;
}

// If there's a trainer, load it in at $7000
void rom_loadtrainer(unsigned char **rom, rominfo_t *rominfo) {
  if (rominfo->flags & ROM_FLAG_TRAINER) {
    //      fread(rominfo->sram + TRAINER_OFFSET, TRAINER_LENGTH, 1, fp);
    memcpy(rominfo->sram + TRAINER_OFFSET, *rom, TRAINER_LENGTH);
    rom += TRAINER_LENGTH;
    /// nes_log_printf("Read in trainer at $7000\n");
  }
}

int rom_loadrom(unsigned char **rom, rominfo_t *rominfo) {
  rominfo->rom = *rom;
  *rom += ROM_BANK_LENGTH * rominfo->rom_banks;

  // If there's VROM, allocate and stuff it in
  if (rominfo->vrom_banks) {
    rominfo->vrom = *rom;
    *rom += VROM_BANK_LENGTH * rominfo->vrom_banks;
  } else {
    if (NULL == rominfo->vram) rominfo->vram = (uint8_t *)malloc(VRAM_LENGTH);
    if (NULL == rominfo->vram) {
      /// gui_sendmsg(GUI_RED, "Could not allocate space for VRAM");
      return -1;
    }
    memset(rominfo->vram, 0, VRAM_LENGTH);
  }

  return 0;
}

int rom_getheader(unsigned char **rom, rominfo_t *rominfo) {
  inesheader_t head;
  uint8_t reserved[RESERVED_LENGTH];
  uint8_t header_dirty;

  // Read in the header
  debug("Head:  %d\n", (*rom)[0]);
  debug("  %d\n", (*rom)[1]);
  debug("  %d\n", (*rom)[2]);
  debug("  %d\n", (*rom)[3]);

  memcpy(&head, *rom, sizeof(head));
  *rom += sizeof(head);

  if (memcmp(head.ines_magic, ROM_INES_MAGIC, 4)) {
    return -1;
  }

  rominfo->rom_banks = head.rom_banks;
  rominfo->vrom_banks = head.vrom_banks;
  // iNES assumptions
  rominfo->sram_banks = 8;  // 1kB banks, so 8KB
  rominfo->vram_banks = 1;  // 8kB banks, so 8KB
  rominfo->mirror =
      (head.rom_type & ROM_MIRRORTYPE) ? MIRROR_VERT : MIRROR_HORIZ;
  rominfo->flags = 0;
  if (head.rom_type & ROM_BATTERY) rominfo->flags |= ROM_FLAG_BATTERY;
  if (head.rom_type & ROM_TRAINER) rominfo->flags |= ROM_FLAG_TRAINER;
  if (head.rom_type & ROM_FOURSCREEN) rominfo->flags |= ROM_FLAG_FOURSCREEN;
  // TODO: fourscreen a mirroring type?
  rominfo->mapper_number = head.rom_type >> 4;

  // Do a compare - see if we've got a clean extended header
  memset(reserved, 0, RESERVED_LENGTH);
  if (0 == memcmp(head.reserved, reserved, RESERVED_LENGTH)) {
    // We were clean
    header_dirty = 0;
    rominfo->mapper_number |= (head.mapper_hinybble & 0xF0);
  } else {
    header_dirty = 1;

    // @!?#@! DiskDude.
    if (('D' == head.mapper_hinybble) &&
        (0 == memcmp(head.reserved, "iskDude!", 8))) {
      /// nes_log_printf("`DiskDude!' found in ROM header, ignoring high mapper
      /// nybble\n");
    } else {
      /// nes_log_printf("ROM header dirty, possible problem\n");
      rominfo->mapper_number |= (head.mapper_hinybble & 0xF0);
    }
    /// rom_adddirty(rominfo->filename);
  }
  // Check for VS unisystem mapper
  if (99 == rominfo->mapper_number) rominfo->flags |= ROM_FLAG_VERSUS;
  return 0;
}

/* Free a ROM */
void rom_free(rominfo_t **rominfo) {
  if (NULL == *rominfo) {
    debug("ROM not loaded");
    return;
  }

  /*  if ((*rominfo)->sram)
      free((*rominfo)->sram);
    Serial.println("sram");    */
  /*  if ((*rominfo)->rom)
      free((*rominfo)->rom);
    Serial.println("rom");
    if ((*rominfo)->vrom)
      free((*rominfo)->vrom);
    Serial.println("vrom");
    if ((*rominfo)->vram)
      free((*rominfo)->vram);
    Serial.println("vram"); */

  free(*rominfo);
}