#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

// KEY BUTTONS PINS:
#define PIN_UP 35     // SVN
#define PIN_DOWN 35   // IO35
#define PIN_LEFT 34   // SVP
#define PIN_RIGHT 12  // TDI
// todo: should be removed
#define PIN_A 2        // IO2
#define PIN_B 14       // TMS
#define PIN_START 15   // TDO
#define PIN_SELECT 13  // TCK

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

#endif  // CONTROLS_H