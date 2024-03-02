#ifndef SDK_DURTY_REGION_TRACKER_H
#define SDK_DURTY_REGION_TRACKER_H

#include <stdint.h>
#include <utils.h>

#include <array>

#include "sdk_configurator.h"

/**
 * @brief Its initial idea and later can be modified.
 * Resolution of screen can vary from 160x120 .. 8x6 depending on pixel
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

// structure representing line of max 160 pixels (bytes of each Segment)
class Line {
 private:
  uint32_t first = 0;
  uint32_t second = 0;
  uint32_t third = 0;
  uint32_t fourth = 0;
  uint32_t fifth = 0;

 public:
  void setPixelOnLine(uint8_t pos);
  bool isPixelSetOnLine(uint8_t pos);
};

class DurtyRegionTracker {
 private:
  const Line emptyLine;
  bool isAtLeastOnePixelDurty = false;

 public:
  std::array<Line, HEIGHT_IN_V_PIXELS> lines;

  DurtyRegionTracker() {
    lines.fill(emptyLine);
    isAtLeastOnePixelDurty = false;
  }

  void resetDurtyRegions() {
    lines.fill(emptyLine);
    isAtLeastOnePixelDurty = false;
  }

  void setDurtyRegion(uint8_t left, uint8_t top, uint8_t right,
                      uint8_t bottom) {
    for (uint8_t y = top; y <= bottom; ++y) {
      Line *line = &(lines[y]);
      for (uint8_t x = left; x <= right; ++x) {
        line->setPixelOnLine(x);
      }
    }
    isAtLeastOnePixelDurty = true;
  }

  void foreachDurtyPixel(void (*callback)(uint8_t x, uint8_t y)) {
    for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
      Line *line = &(lines[y]);
      for (uint8_t x = 0; x < WIDTH_IN_V_PIXELS; ++x) {
        if (line->isPixelSetOnLine(x) == true) {
          callback(x, y);
        }
      }
    }
  }
};

#endif  // SDK_DURTY_REGION_TRACKER_H