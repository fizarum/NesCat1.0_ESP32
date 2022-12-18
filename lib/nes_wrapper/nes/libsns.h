#ifndef LIBSNS_H
#define LIBSNS_H

#include <stdint.h>

#define TAG_LENGTH 4

struct mapper1Data {
  uint8_t registers[4];
  uint8_t latch;
  uint8_t numberOfBits;
};

struct mapper4Data {
  uint8_t irqCounter;
  uint8_t irqLatchCounter;
  uint8_t irqCounterEnabled;
  uint8_t last8000Write;
};

struct mapper5Data {
  // needed for some compilers; remove if any members are added
  uint8_t dummy;
};

struct mapper6Data {
  uint8_t irqCounter;
  uint8_t irqLatchCounter;
  uint8_t irqCounterEnabled;
  uint8_t last43FEWrite;
  uint8_t last4500Write;
};

struct mapper9Data {
  uint8_t latch[2];
  uint8_t lastB000Write;
  uint8_t lastC000Write;
  uint8_t lastD000Write;
  uint8_t lastE000Write;
};

struct mapper10Data {
  uint8_t latch[2];
  uint8_t lastB000Write;
  uint8_t lastC000Write;
  uint8_t lastD000Write;
  uint8_t lastE000Write;
};

struct mapper16Data {
  uint8_t irqCounterLowByte;
  uint8_t irqCounterHighByte;
  uint8_t irqCounterEnabled;
};

struct mapper17Data {
  uint8_t irqCounterLowByte;
  uint8_t irqCounterHighByte;
  uint8_t irqCounterEnabled;
};

struct mapper18Data {
  uint8_t irqCounterLowByte;
  uint8_t irqCounterHighByte;
  uint8_t irqCounterEnabled;
};

struct mapper19Data {
  uint8_t irqCounterLowByte;
  uint8_t irqCounterHighByte;
  uint8_t irqCounterEnabled;
};

struct mapper21Data {
  uint8_t irqCounter;
  uint8_t irqCounterEnabled;
};

struct mapper24Data {
  uint8_t irqCounter;
  uint8_t irqCounterEnabled;
};

struct mapper40Data {
  uint8_t irqCounter;
  uint8_t irqCounterEnabled;
};

struct mapper69Data {
  uint8_t irqCounterLowByte;
  uint8_t irqCounterHighByte;
  uint8_t irqCounterEnabled;
};

struct mapper90Data {
  uint8_t irqCounter;
  uint8_t irqLatchCounter;
  uint8_t irqCounterEnabled;
};

struct mapper224Data {
  uint8_t chrRamWriteable;
};

struct mapper225Data {
  uint8_t prgSize;
  uint8_t registers[4];
};

struct mapper226Data {
  uint8_t chrRamWriteable;
};

struct mapper228Data {
  uint8_t prgChipSelected;
};

struct mapper230Data {
  uint8_t numberOfResets;
};

typedef struct _SnssFileHeader {
  char tag[TAG_LENGTH + 1];
  unsigned int numberOfBlocks;
} SnssFileHeader;

// this block appears before every block in the SNSS file
typedef struct _SnssBlockHeader {
  char tag[TAG_LENGTH + 1];
  unsigned int blockVersion;
  unsigned int blockLength;
} SnssBlockHeader;

#define MAPPER_BLOCK_LENGTH 0x98
typedef struct _SnssMapperBlock {
  unsigned short prgPages[4];
  unsigned short chrPages[8];

  union _extraData {
    uint8_t mapperData[128];
    struct mapper1Data mapper1;
    struct mapper4Data mapper4;
    struct mapper5Data mapper5;
    struct mapper6Data mapper6;
    struct mapper9Data mapper9;
    struct mapper10Data mapper10;
    struct mapper16Data mapper16;
    struct mapper17Data mapper17;
    struct mapper18Data mapper18;
    struct mapper19Data mapper19;
    struct mapper21Data mapper21;
    struct mapper24Data mapper24;
    struct mapper40Data mapper40;
    struct mapper69Data mapper69;
    struct mapper90Data mapper90;
    struct mapper224Data mapper224;
    struct mapper225Data mapper225;
    struct mapper226Data mapper226;
    struct mapper228Data mapper228;
    struct mapper230Data mapper230;
  } extraData;
} SnssMapperBlock;

#endif  // LIBSNS_H