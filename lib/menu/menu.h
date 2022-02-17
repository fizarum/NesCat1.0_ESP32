#ifndef MENU_H
#define MENU_H

#include <stdint.h>

/**
 * @brief update active menu based on controls state
 * @param isJoyMovedHorizontally horizontal position of joystick: 1 for left, 2
 * for right, 0 for no movement
 * @return uint8_t new menu index
 */
void updateActiveMenuIndex(uint8_t isJoyMovedHorizontally);
const char* getCurrentMenuTitle();
uint8_t getCurrentMenuIndex();

#endif  // MENU_H