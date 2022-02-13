#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

// KEY BUTTONS PINS:
#define PIN_UP 39      // SVN
#define PIN_DOWN 35    // IO35
#define PIN_LEFT 36    // SVP
#define PIN_RIGHT 12   // TDI
#define PIN_A 2        // IO2
#define PIN_B 14       // TMS
#define PIN_START 15   // TDO
#define PIN_SELECT 13  // TCK

#define JOY_MAX_VAL 4096
// normal postition of joystic - no keys pressed
#define JOY_NORMAL_VAL JOY_MAX_VAL / 2
// minimal step for joystic to register press event
#define JOY_TRESHOLD JOY_MAX_VAL / 10

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

void controlsInit();
void controlsUpdate();

#endif  // CONTROLS_H