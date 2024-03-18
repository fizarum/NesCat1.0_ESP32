#ifndef SDK_DURTY_REGION_TRACKER_H
#define SDK_DURTY_REGION_TRACKER_H

#include <stdint.h>

#ifdef PORT_SDK
#include <stdio.h>
#include <string.h>
#endif  // PORT_SDK

#include <array>

#include "../utils/utils.h"
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
#ifdef PORT_SDK
  uint8_t _buff_len = DISPLAY_WIDTH + 1;
  char *_durty_region_tracker_buff;
#endif  // PORT_SDK

 public:
  Line() {
#ifdef PORT_SDK
    _durty_region_tracker_buff = new char[_buff_len];
    memset(_durty_region_tracker_buff, 0, _buff_len);
#endif  // PORT_SDK
  }
  ~Line() {
#ifdef PORT_SDK
    delete[] _durty_region_tracker_buff;
#endif  // PORT_SDK
  }

  void setPixelOnLine(uint8_t pos) {
    if (pos < 32) {
      this->first = bit::setBit32(this->first, pos);
      return;
    }
    if (pos < 64) {
      this->second = bit::setBit32(this->second, pos);
      return;
    }
    if (pos < 96) {
      this->third = bit::setBit32(this->third, pos);
      return;
    }
    if (pos < 128) {
      this->fourth = bit::setBit32(this->fourth, pos);
      return;
    }

    this->fifth = bit::setBit32(this->fifth, pos);
  }

  bool isPixelSetOnLine(uint8_t pos) {
    if (pos < 32) return bit::isBitSet32(this->first, pos);
    if (pos < 64) return bit::isBitSet32(this->second, pos);
    if (pos < 96) return bit::isBitSet32(this->third, pos);
    if (pos < 128) return bit::isBitSet32(this->fourth, pos);
    return bit::isBitSet32(this->fifth, pos);
  }

  void resetLine() {
    this->first = 0;
    this->second = 0;
    this->third = 0;
    this->fourth = 0;
    this->fifth = 0;
  }

  /**
   * @brief Get information if line contains some pixels
   *
   * @return true if at least one pixel is set
   * @return false if line is clear
   */
  bool hasAnyPixelSet() {
    return this->first > 0 || this->second > 0 || this->third > 0 ||
           this->fourth > 0 || this->fifth > 0;
  }

#ifdef PORT_SDK
  void const inline printLine(char durtyPixel = 219) {
    memset(_durty_region_tracker_buff, 0, _buff_len);
    for (uint8_t pos = 0; pos <= _buff_len; ++pos) {
      _durty_region_tracker_buff[pos] =
          this->isPixelSetOnLine(pos) == true ? durtyPixel : ' ';
    }
    printf("%s\n", _durty_region_tracker_buff);
  }
#endif
};

class DurtyRegionTracker {
 private:
  const Line emptyLine;
  bool isAtLeastOnePixelDurty = false;
  std::array<Line, HEIGHT_IN_V_PIXELS> lines;

 public:
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

  void redrawDurtyPixels(void (*callback)(Line *line, uint8_t y)) {
    if (isAtLeastOnePixelDurty == false) return;

    for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
      Line *line = &(lines[y]);

      if (line->hasAnyPixelSet() == true) {
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