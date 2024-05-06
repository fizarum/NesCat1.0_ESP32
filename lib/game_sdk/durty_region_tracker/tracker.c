#include "tracker.h"

#include <stdlib.h>

#ifdef PORT_SDK
#include <stdio.h>
#include <string.h>
#endif  // PORT_SDK

typedef struct DRTracker_t {
  bool isAtLeastOnePixelDurty;
  DRTLine_t *lines[HEIGHT_IN_V_PIXELS];
} DRTracker_t;

DRTracker_t *DRTrackerCreate() {
  DRTracker_t *tracker = (DRTracker_t *)malloc(sizeof(DRTracker_t));

  for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
    tracker->lines[index] = DRTLineCreate();
  }

  tracker->isAtLeastOnePixelDurty = false;

  return tracker;
}

void DRTrackerDestroy(DRTracker_t *tracker) {
  for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
    DRTLine_t *line = tracker->lines[index];
    DRTLineDestroy(line);
  }
  free(tracker);
}

void DRTrackerClear(DRTracker_t *tracker) {
  for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
    DRTLine_t *line = tracker->lines[index];
    DRTLineReset(line);
  }
  tracker->isAtLeastOnePixelDurty = false;
}

void DRTrackerSetDurtyRegion(DRTracker_t *tracker, uint8_t left, uint8_t top,
                             uint8_t right, uint8_t bottom) {
  for (uint8_t y = top; y <= bottom; ++y) {
    DRTLine_t *line = tracker->lines[y];
    for (uint8_t x = left; x <= right; ++x) {
      DRTLineSetPixel(line, x);
    }
  }
  tracker->isAtLeastOnePixelDurty = true;
}

void DRTrackerGetDurtyRegions(const DRTracker_t *tracker,
                              void (*callback)(DRTLine_t *line,
                                               uint8_t lineIndex)) {
  if (tracker->isAtLeastOnePixelDurty == false) return;

  for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
    DRTLine_t *line = tracker->lines[y];
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

uint8_t _buff_len = DISPLAY_WIDTH + 1;
char _durty_region_tracker_buff[DISPLAY_WIDTH + 1];

void const inline printLine(const DRTLine_t *line, char durtyPixel = 219) {
  memset(_durty_region_tracker_buff, 0, _buff_len);
  for (uint8_t pos = 0; pos <= _buff_len; ++pos) {
    const char pixel = DRTLineIsPixelSet(line, pos) == true ? durtyPixel : ' ';
    _durty_region_tracker_buff[pos] = pixel;
  }
  printf("%s\n", _durty_region_tracker_buff);
}

void fillString(char *buff, size_t len, char symbol) {
  memset(buff, 0, len);
  memset(buff, symbol, len - 1);
}

void DRTrackerPrintDebugInfo(DRTracker_t *tracker) {
  if (tracker->isAtLeastOnePixelDurty == false) return;

  fillString(seg31, 31, segItem);

  printf("%s%c%s%c%s%c%s%c%s\n", seg31, separator, seg31, separator, seg31,
         separator, seg31, separator, seg31);

  for (uint8_t lineNum = 0; lineNum < _print_lines; ++lineNum) {
    DRTLine_t *line = tracker->lines[lineNum];
    printLine(line);
  }

  printf("%s%c%s%c%s%c%s%c%s\n", seg31, separator, seg31, separator, seg31,
         separator, seg31, separator, seg31);
}
#endif  // PORT_SDK