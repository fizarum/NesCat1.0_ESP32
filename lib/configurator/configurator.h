// enable or disable debugging
// currently, it enables debug() function
#define DEBUG_ON

// devices section
// list of registered devices and their ids
#define DEVICE_DISPLAY_ID 100
#define DEVICE_AUDIO_ID 101
#define DEVICE_JOYSTICK_ID 102
#define DEVICE_STORAGE_ID 103

// flags which device should be enabled
#define DISPLAY_ON
// #define AUDIO_ON
#define JOYSTICK_ON
#define STORAGE_ON

// display options
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_ORIENTATION 1
#define DISPLAY_H_CENTER DISPLAY_WIDTH / 2
#define DISPLAY_V_CENTER DISPLAY_HEIGHT / 2

// backlight values
// backlight off
#define BL_LEVEL_OFF 0
// min backlight
#define BL_LEVEL_MIN 1
// full light
#define BL_LEVEL_MAX 255
// default backlight value
#define BL_LEVEL_DEFAULT 40