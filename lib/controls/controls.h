/**
sony joystick map used in project:

        up             triangle
    left  right     square  circle
       down             cross

 */

#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

extern uint8_t JOY_UP;
extern uint8_t JOY_DOWN;
extern uint8_t JOY_LEFT;
extern uint8_t JOY_RIGHT;
extern uint8_t JOY_CROSS;
extern uint8_t JOY_SQUARE;
extern uint8_t JOY_CIRCLE;
extern uint8_t JOY_TRIANGLE;
//(START)
extern uint8_t JOY_SHARE;
//(SELECT)
extern uint8_t JOY_OPTIONS;

void controlsInit(void (*onKeysCallback)(uint16_t),
                  void (*onJoystickMovedCallback)(uint16_t));
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