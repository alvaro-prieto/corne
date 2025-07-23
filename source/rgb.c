#include <lib/lib8tion/lib8tion.h>
#include <stdint.h>
#include "shared.h"
#include "rgb.h"
#include "sync.h"

//═══════════════════════════════════════════════════════════════
//	   RGB MAIN
//═══════════════════════════════════════════════════════════════
/*
          C-0   C-1   C-2   C-3   C-4   C-5

        .-----------------------------------.
R-0     | 24  | 23  | 18  | 17  |  10 |  9  |
        |-----+----[2]----+----[1]---[0]----|
R-1     | 25  | 22  | 19  | 16  |  11 |  8  |
        |-----+----[3]----+-----+-----+-----|
R-2     | 26  | 21  | 20  | 15  |  12 |  7  |
        `-----+-----+-----+----[4]---[5]----.
R-3                       \ 14 | 13  |  6  |
                           `--------------´

*/

static const int8_t led_map[NUM_ROWS][NUM_COLUMNS] = {
    {24, 23, 18, 17, 10, 9},   // row 1
    {25, 22, 19, 16, 11, 8},   // row 2
    {26, 21, 20, 15, 12, 7},   // row 3
    {-1, -1, -1, 14, 13, 6}    // row 4
};

extern bool master;
extern bool kb_lock;

// RGB State variables
static enum rgb_reactive current_reactive = RGB_NO_REACTIVE;
static enum rgb_notification current_notification = RGB_NO_NOTIFICATION;
static enum rgb_animation current_animation= RGB_NO_ANIMATION;
static enum rgb_theme current_theme = RGB_NO_THEME;
static bool rgb_on = true;
static uint8_t number_of_active_indicators = 0;
static enum rgb_indicator active_indicators[ MAX_NUMBER_OF_INDICATORS ];
static bool rgb_dirty = true;
static bool rgb_suspended = false;
static uint32_t t = 0;                      // Animation elapsed time starting on 0 (in ms)
static uint16_t dt = 0;                     // Delta-time: time since last frame (in ms)
static uint32_t last_render_time = 0;       // Timestamp of the last rendered frame
static uint32_t animation_start_time = 0;   // Timestamp of the current animation's start

// buffer_1d and buffer_2d were created to store data used in various animations.
// Both can be used independently or together, depending on the needs of each effect.
// The main difference is that buffer_1d is a linear storage, while buffer_2d is
// structured in rows and columns for convenience in spatially organized animations.
// If an effect requires overlays or additional data, both buffers can be combined
// for greater flexibility in animation representation.

static uint16_t buffer_1d[ NUM_ROWS * NUM_COLUMNS];
static uint16_t buffer_2d[ NUM_ROWS ][ NUM_COLUMNS ];
static uint16_t buffer_small[ NUM_COLUMNS ];
static RGB palette[ PALETTE_LENGTH ];

#include "rgb_utils.c"


//	FUNCTIONS
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Reset time for the current animation.
static void reset_time(void){
    last_render_time = 0;
}

// Determines whether the RGB main loop should be disabled or remain running.
static void update_rgb_state(void){
    bool rgb_in_use = (
        current_reactive != RGB_NO_REACTIVE ||
        current_animation != RGB_NO_ANIMATION ||
        current_notification != RGB_NO_NOTIFICATION ||
        current_theme != RGB_NO_THEME ||
        number_of_active_indicators > 0
    );
    #ifdef RGB_DEBUG
        rgb_in_use = true;
    #endif
    if(rgb_in_use != rgb_on){
        rgb_on ? disable_rgb() : enable_rgb();
        rgb_on = rgb_in_use;
        //rgb_on ? xprintf("Enable RGB \n") : xprintf("Disable RGB \n");
    }
}

// Flag for pending visual updates
static void dirty(void){
    rgb_dirty = true;
    update_rgb_state();
}

// Increase / decrease the current LED brightness.
// direction can be either 1 or -1
void rgb_brightness( int8_t direction ){
    //to-do
    rgb_dirty = true;
}


//	RGB SUBMODULES
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

#include "rgb_indicators.c"
#include "rgb_animations.c"
#include "rgb_themes.c"
#include "rgb_notifications.c"
#include "rgb_reactive.c"


//	RGB LIFECYCLE
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Executed when the keyboard is initialized
void post_init_rgb(void){
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_no_effect);
    #ifdef DISABLE_UNDERGLOW
        // no underlight required since my keyboard has an opaque case
        rgb_matrix_set_flags_noeeprom(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER);
    #endif
    reset_indicators();
    enable_rgb();
    turn_off_all_leds();
}

// Executed in both master and slave to change the RGB suspend state
void rgb_set_suspend(bool suspend){
    if(suspend){
        disable_rgb();
        rgb_on = false;
    }else if(rgb_suspended){
        reset_time();
        dirty();
    }
    rgb_suspended = suspend;
}



//	DRAWING DYNAMIC EFFECTS
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Draw dynamic effects, such as notifications and animations
// It returns true if any dynamic effect is active.
bool draw_dynamic_effect(void) {
    if (current_notification == RGB_NO_NOTIFICATION &&
        current_animation == RGB_NO_ANIMATION &&
        current_reactive == RGB_NO_REACTIVE) {
        return false;
    }
    uint32_t now = sync_timer_read32();
    if (last_render_time == 0) {
        animation_start_time = now;
        last_render_time = now;
    }
    dt = (uint16_t)(now - last_render_time);
    #ifdef RGB_MATRIX_LED_FLUSH_LIMIT
    if (RGB_MATRIX_LED_FLUSH_LIMIT > 0 && dt < RGB_MATRIX_LED_FLUSH_LIMIT) {
        return true; // Limit frame rate
    }
    #endif
    t = now - animation_start_time;
    if (current_notification != RGB_NO_NOTIFICATION) {
        draw_current_notification();
    } else if (current_animation != RGB_NO_ANIMATION) {
        draw_current_animation();
    } else if (current_reactive != RGB_NO_REACTIVE) {
        draw_current_reactive_animation();
    }
    last_render_time = now;
    return true;
}

//	RGB MAIN LOOP
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// RGB main loop. Executes only when rgb_matrix is enabled.
// Optimized for resource efficiency—RGB activates only when there are pending changes.
bool rgb_matrix_indicators_user(void) {
    if( !draw_dynamic_effect() && rgb_dirty ){
        xprintf("RGB is dirty \n");
        uprintf("Hola\n");
        turn_off_all_leds();
        if( kb_lock ) return false; // Skip theme rendering if the keyboard is locked.
        if( current_theme != RGB_NO_THEME ){
            draw_current_theme();
        }
        draw_indicators();
        rgb_dirty = false;
    }
    return false;
}


