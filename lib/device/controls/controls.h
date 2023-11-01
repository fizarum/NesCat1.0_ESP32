#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdint.h>

#include "keymap.h"

void controlsInit(void (*onInputCallbackPtr)(uint16_t));
void controlsUpdate();

bool isLeftPressed();
bool isRightPressed();
bool isDownPressed();
bool isUpPressed();

bool isSelectPressed();
bool isStartPressed();
bool isMenuPressed();

bool isAPressed();
bool isBPressed();
bool isXPressed();
bool isYPressed();

bool isLeftTriggerPressed();
bool isRightTriggerPressed();

#endif  // CONTROLS_H