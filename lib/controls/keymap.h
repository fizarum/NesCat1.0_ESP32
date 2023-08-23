#ifndef KEYMAP_H
#define KEYMAP_H

/**
sony like keymap used in project:

  left_trigger       right_trigger

        up             triangle (X)
    left  right     square(Y)  circle(A)
       down             cross(B)

      select  start  menu
 */

// gpio definitions
#define GPIO_BUTTON_LEFT 10
#define GPIO_BUTTON_RIGHT 11
#define GPIO_BUTTON_UP 9
#define GPIO_BUTTON_DOWN 12

#define GPIO_BUTTON_A 5
#define GPIO_BUTTON_B 3
#define GPIO_BUTTON_X 6
#define GPIO_BUTTON_Y 4

#define GPIO_BUTTON_SELECT 13
#define GPIO_BUTTON_START 14
#define GPIO_BUTTON_MENU 15

#define GPIO_TRIGGER_LEFT 8
#define GPIO_TRIGGER_RIGHT 7

#endif  // KEYMAP_H