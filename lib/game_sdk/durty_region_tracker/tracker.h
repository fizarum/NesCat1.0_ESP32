#ifndef SDK_DURTY_REGION_TRACKER_H
#define SDK_DURTY_REGION_TRACKER_H

#include <stdint.h>

#ifdef PORT_SDK
#include <stdio.h>
#include <string.h>
#endif  // PORT_SDK

#include "../utils/utils.h"
#include "line.h"
#include "sdk_configurator.h"

/**
 * @brief Its initial idea and later can be modified.
 * Resolution of screen can vary from 8x6 to 160x120 depending on pixel
 * size. For flexibility of current solution we have count that we always
 * working on the biggest available resolution: 160x120. Based on this info we
 * need some way to mark "durty" pixels for redraw in next draw call.
 * The simplest way is to make an array of numbers, each bit of which keeps
 * "durty" pixel data. If we have uint8_t which have 8 bits we can represent 8
 * pixels, thus 195 (or b1100 0011 or 0xC3) can refer to first, second, seventh
 * and eighth pixels as "durty" onces. Because we have limits in big integers,
 * we can take uint32_t as such pixel representer. But 160 (width) or even 120
 * (height) are too big for uint32_t.
 * So, to minimize pixels wasting we can select next way: each line has 5 of
 * uint32_t numbers to represent one line of 160 pixels. And we need 120 such
 * numbers.
 *
 * Briefly, entire screen is: array<Line, 120> - 120 lines, where Line is:
 * array<uint32_t, 5> 5 numbers of uint32_t
 */

class DurtyRegionTracker {
 private:
  bool isAtLeastOnePixelDurty = false;
  DRTLine_t *lines[HEIGHT_IN_V_PIXELS];

 public:
  DurtyRegionTracker() {
    for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
      lines[index] = DRTLineCreate();
    }

    isAtLeastOnePixelDurty = false;
  }

  void resetDurtyRegions() {
    for (uint8_t index = 0; index < HEIGHT_IN_V_PIXELS; ++index) {
      DRTLine_t *line = lines[index];
      if (DRTLineHasAnyPixelSet(line)) {
        DRTLineReset(line);
      }
    }
    isAtLeastOnePixelDurty = false;
  }

  void setDurtyRegion(uint8_t left, uint8_t top, uint8_t right,
                      uint8_t bottom) {
    for (uint8_t y = top; y <= bottom; ++y) {
      DRTLine_t *line = lines[y];
      for (uint8_t x = left; x <= right; ++x) {
        DRTLineSetPixel(line, x);
      }
    }
    isAtLeastOnePixelDurty = true;
  }

  void redrawDurtyPixels(void (*callback)(DRTLine_t *line, uint8_t y)) {
    if (isAtLeastOnePixelDurty == false) return;

    for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
      DRTLine_t *line = lines[y];
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
#endif  // PORT_SDK

  void printDebugInfo() {
#ifdef PORT_SDK
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
#endif  // PORT_SDK
  }
};

#endif  // SDK_DURTY_REGION_TRACKER_H