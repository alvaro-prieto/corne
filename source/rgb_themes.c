#include "rgb.h"

//═════════════════════════════════════════════════════════
//      STATIC RGB THEMES
//═════════════════════════════════════════════════════════

// Rotate the current theme. Direction can be either 1 or -1
void rgb_rotate_theme( int8_t direction ){
    current_theme = (current_theme + direction + RGB_NO_THEME + 1) % (RGB_NO_THEME + 1);
    rgb_set_theme(current_theme);
}

// Set a specific rgb theme
void rgb_set_theme (int8_t theme){
    rgb_set_theme_no_sync( theme );
    sync(SYNC_RGB_THEME, theme, 0);
}

// Set a specific rgb theme in the current half. No sync
void rgb_set_theme_no_sync (int8_t theme){
    rgb_disable_all_except( RGB_THEME );
    current_theme = theme;
    dirty();
}

// Draw the current rgb background theme
static void draw_current_theme(void){
    HSV hsv;
    hsv.s = MAX_INT8;
    hsv.v = MAX_BRIGHT;
    for(uint8_t i=FIRST_TOP_LED; i<=LAST_TOP_LED; i++){
        switch (current_theme){
            case RGB_THEME_1:
                hsv.h = MAX_INT8 - i*10;
                break;
            case RGB_THEME_2:
                hsv.h = 0 - i*2;
                break;
            case RGB_FLASHING:
                hsv.h = 50 - i*1.5;
                break;
            case RGB_MILK:
                hsv.s = 0;
                break;
            default:
                hsv.v = 0;
            break;
        }
        RGB rgb = hsv_to_rgb( hsv );
        set_led( mirror_led_index(i), rgb.r, rgb.g, rgb.b);
    }
}

