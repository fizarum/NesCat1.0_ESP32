/**
sony joystick map used in project:

        up             triangle
    left  right     square  circle
       down             cross

 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

void controlsInit(void (*onInputCallbackPtr)(uint16_t));
void controlsUpdate();

bool isTrianglePressed(uint16_t state);
bool isCirclePressed(uint16_t state);
bool isCrossPressed(uint16_t state);
bool isSquarePressed(uint16_t state);

bool isStartPressed(uint16_t state);
bool isSelectPressed(uint16_t state);

bool isLeftPressed(uint16_t state);
bool isRightPressed(uint16_t state);
bool isDownPressed(uint16_t state);
bool isUpPressed(uint16_t state);

#endif  // CONTROLS_H