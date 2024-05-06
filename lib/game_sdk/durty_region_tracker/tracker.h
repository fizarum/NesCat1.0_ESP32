#ifndef SDK_DURTY_REGION_TRACKER_H
#define SDK_DURTY_REGION_TRACKER_H

#include <stdint.h>

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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DRTracker_t DRTracker_t;

DRTracker_t *DRTrackerCreate();
void DRTrackerDestroy(DRTracker_t *tracker);

/**
 * @brief Clear durty regions of tracker. This resets tracker to initial state
 *
 * @param tracker to reset
 */
void DRTrackerClear(DRTracker_t *tracker);

/**
 * @brief Sets region as durty - which will be redrawn on next draw session
 */
void DRTrackerSetDurtyRegion(DRTracker_t *tracker, uint8_t left, uint8_t top,
                             uint8_t right, uint8_t bottom);

/**
 * @brief Obtain all durty regions line by line. For each durty line callback is
 * triggered
 *
 * @param tracker
 * @param callback - Triggered callback with durty line and its index
 */
void DRTrackerGetDurtyRegions(const DRTracker_t *tracker,
                              void (*callback)(DRTLine_t *line,
                                               uint8_t lineIndex));

#ifdef PORT_SDK
void DRTrackerPrintDebugInfo(DRTracker_t *tracker);
#endif  // PORT_SDK

#ifdef __cplusplus
}
#endif

#endif  // SDK_DURTY_REGION_TRACKER_H