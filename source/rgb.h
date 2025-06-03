#pragma once
#include <stdint.h>
#include "sync.h"

#define FIRST_TOP_LED 6
#define LAST_TOP_LED 26
#define TOP_LED_COUNT_HALF 21
#define FIRST_LED_SLAVE 27
#define FIRST_TOP_LED_SLAVE (LAST_TOP_LED + 1 + FIRST_TOP_LED)
#define LAST_TOP_LED_SLAVE (FIRST_TOP_LED_SLAVE + TOP_LED_COUNT_HALF - 1)
#define MAX_INT16 65535
#define MAX_INT8 255
#define MAX_BRIGHT RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define MAX_INDICATOR_LENGTH 10
#define MAX_NUMBER_OF_INDICATORS 5
#define NOTIFICATION_DURATION 1200
#define PALETTE_LENGTH 512
#define CENTER_X 50
#define CENTER_Y 32
#define THUMB_X 90
#define THUMB_Y 55
#define NUM_COLUMNS 6
#define NUM_ROWS 4


#define safe(x) ((x) > MAX_BRIGHT ? MAX_BRIGHT : (x))

// RGB type flags
enum rgb_types{
    RGB_INDICATOR    = 1 << 0,
    RGB_NOTIFICATION = 1 << 1,
    RGB_ANIMATION    = 1 << 2,
    RGB_REACTIVE     = 1 << 3,
    RGB_THEME        = 1 << 4
};

// The indicators are groups of keys that light up to notify of a state
// Enum value must be the index of the first element in the leds[] group
enum rgb_indicator{
    RGB_NONE = -1,
    RGB_HYPER = 0,
    RGB_CAPS_LOCK = 1,
    RGB_CAPS_WORD = 3,
    RGB_EXTRAS = 4,
    RGB_HYPER_LOCK = 5,
    RGB_NUM_LOCK = 7,
};

// Small RGB animations for visual notifications
enum rgb_notification{
    RGB_WINDOWS_NOTIFICATION = 0,
    RGB_MAC_NOTIFICATION,
    RGB_LOCK_NOTIFICATION,
    RGB_RING_NOTIFICATION,
    RGB_CAPS_WORD_NOTIFICATION,
    RGB_NO_NOTIFICATION
};

enum rgb_animation {
    RGB_LAVA_ANIMATION = 0,
    RGB_PLASMA_ANIMATION,
    RGB_MATRIX_ANIMATION,
    RGB_FIRE_ANIMATION,
    RGB_BOREAL_ANIMATION,
    RGB_NO_ANIMATION
};

enum rgb_reactive{
    RGB_REACTIVE_DIFFUSION = 0,
    RGB_REACTIVE_DIFFUSION_FIRE,
    RGB_REACTIVE_HEATMAP,
    RGB_REACTIVE_LONG_HEATMAP,
    RGB_REACTIVE_MAGIC,
    RGB_NO_REACTIVE
};

// Static backgrounds to give a touch of color to the keyboard
enum rgb_theme{
    RGB_THEME_1 = 0,
    RGB_THEME_2,
    RGB_FLASHING,
    RGB_MILK,
    RGB_NO_THEME
};


void post_init_rgb( void );
void rgb_set_indicator( enum rgb_indicator, bool on );
void rgb_set_notification( enum rgb_notification notification );
void rgb_set_theme( int8_t theme );
void rgb_set_theme_no_sync( int8_t theme );
void rgb_set_animation(enum rgb_animation animation);
void rgb_set_reactive(enum rgb_reactive reactive);
void rgb_set_suspend( bool suspend );
void rgb_keyboard_event( uint8_t col, uint8_t row, bool down);
void rgb_rotate_theme( int8_t direction );
void rgb_rotate_animation(int8_t direction);
void rgb_rotate_reactive( int8_t direction );
void rgb_brightness( int8_t direction ); // TO-DO:  not implmented yet

#ifdef RGB_DEBUG
void debug_slave(uint16_t number1, uint16_t number2);
#endif

