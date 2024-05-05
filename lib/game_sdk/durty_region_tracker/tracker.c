#include "tracker.h"

#include <stdlib.h>

#ifdef PORT_SDK
#include <stdio.h>
#include <string.h>
#endif  // PORT_SDK

typedef struct {
  bool isAtLeastOnePixelDurty;
  DRTLine_t *lines[HEIGHT_IN_V_PIXELS];
} DRTrackerImpl_t;

DRTracker_t *DRTrackerCreate() {
  DRTrackerImpl_t *tracker = malloc(sizeof(DRTrackerImpl_t));

  for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
    tracker->lines[index] = DRTLineCreate();
  }

  tracker->isAtLeastOnePixelDurty = false;

  return (DRTracker_t *)tracker;
}

void DRTrackerDestroy(DRTracker_t *tracker) {
  DRTrackerImpl_t *impl = (DRTrackerImpl_t *)tracker;

  for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
    DRTLine_t *line = impl->lines[index];
    DRTLineDestroy(line);
    free(impl);
  }
}

void DRTrackerClear(DRTracker_t *tracker) {
  DRTrackerImpl_t *impl = (DRTrackerImpl_t *)tracker;

  for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
    DRTLine_t *line = impl->lines[index];
    DRTLineReset(line);
  }
  impl->isAtLeastOnePixelDurty = false;
}

void DRTrackerSetDurtyRegion(DRTracker_t *tracker, uint8_t left, uint8_t top,
                             uint8_t right, uint8_t bottom) {
  DRTrackerImpl_t *impl = (DRTrackerImpl_t *)tracker;

  for (uint8_t y = top; y <= bottom; ++y) {
    DRTLine_t *line = impl->lines[y];
    for (uint8_t x = left; x <= right; ++x) {
      DRTLineSetPixel(line, x);
    }
  }
  impl->isAtLeastOnePixelDurty = true;
}

void DRTrackerGetDurtyRegions(const DRTracker_t *tracker,
                              void (*callback)(DRTLine_t *line,
                                               uint8_t lineIndex)) {
  DRTrackerImpl_t *impl = (DRTrackerImpl_t *)tracker;
  if (impl->isAtLeastOnePixelDurty == false) return;

  for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
    DRTLine_t *line = impl->lines[y];
    if (DRTLineHasAnyPixelSet(line)) {
      callback(line, y);
    }
  }
}

#ifdef PORT_SDK
char seg31[31];
const char vSeparator = 179;
const char separator = 194;
const char segItem = 196;

// lets print only 40 fist lines
uint8_t _print_lines = 40;

void fillString(char *buff, size_t len, char symbol) {
  memset(buff, 0, len);
  memset(buff, symbol, len - 1);
}

void DRTrackerPrintDebugInfo(DRTracker_t *tracker) {
  if (isAtLeastOnePixelDurty == false) return;

  fillString(seg31, 31, segItem);

  printf("%s%c%s%c%s%c%s%c%s\n", seg31, separator, seg31, separator, seg31,
         separator, seg31, separator, seg31);

  for (uint8_t lineNum = 0; lineNum < _print_lines; ++lineNum) {
    Line *line = &(lines[lineNum]);
    line->printLine();
  }

  printf("%s%c%s%c%s%c%s%c%s\n", seg31, separator, seg31, separator, seg31,
         separator, seg31, separator, seg31);
}
#endif  // PORT_SDK