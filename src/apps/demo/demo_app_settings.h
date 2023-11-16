#ifndef DEMO_APP_SETTINGS_H
#define DEMO_APP_SETTINGS_H

/*
 * Virtual pixel size. Bigger pixel size - better performance
 * but smaller resolution
 */

// #define PIXEL_SIZE 2  // 160x120
// #define PIXEL_SIZE 3  // 106x80
// #define PIXEL_SIZE 4  // 80x60
// #define PIXEL_SIZE 5  // 64x48

// the best performance/pixel size value
#define PIXEL_SIZE 8  // 40x30

// #define PIXEL_SIZE 10  // 32x24
// #define PIXEL_SIZE 20  // 16x12
// #define PIXEL_SIZE 40  // 8x6

// display values
#define WIDTH_IN_V_PIXELS DISPLAY_WIDTH / PIXEL_SIZE
#define HEIGHT_IN_V_PIXELS DISPLAY_HEIGHT / PIXEL_SIZE

#define FRAME_BUFFER_SIZE WIDTH_IN_V_PIXELS* HEIGHT_IN_V_PIXELS

// task priorities
#define RENDER_TASK_PRIORITY 2
#define WRITE_FRAME_TASK_PRIORITY 1

#endif  // DEMO_APP_SETTINGS_H