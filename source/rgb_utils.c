//═══════════════════════════════════════════════════════════════
//	RGB AUX FUNCTIONS
//═══════════════════════════════════════════════════════════════

// Turn leds black (clear)
#include "rgb.h"
static void turn_off_all_leds(void){
    rgb_matrix_set_color_all(0, 0, 0);
}

// Clear rgb and disable
static void disable_rgb(void){
    turn_off_all_leds();
    rgb_matrix_disable_noeeprom();
}

// Enable rgb
static void enable_rgb(void){
    rgb_matrix_enable_noeeprom();
}

// Update reactive RGB state according to the current animation
void rgb_update_reactiveness(void) {
    reactive_rgb = current_reactive != RGB_NO_REACTIVE;
}

// Disable all RGB animations except flagged ones. No sync
void rgb_disable_all_except(enum rgb_types keep_types) {
    if (!(keep_types & RGB_REACTIVE))
        current_reactive = RGB_NO_REACTIVE;
    if (!(keep_types & RGB_ANIMATION))
        current_animation = RGB_NO_ANIMATION;
    if (!(keep_types & RGB_NOTIFICATION))
        current_notification = RGB_NO_NOTIFICATION;
    if (!(keep_types & RGB_THEME))
        current_theme = RGB_NO_THEME;
    rgb_update_reactiveness();
}

// Converts the absolute LED index to a relative index within
// the current half of the keyboard (master or slave). 0-index based.
static inline uint8_t to_relative_led_index(uint8_t led_index) {
    return led_index - FIRST_TOP_LED - (master ? 0 : FIRST_LED_SLAVE);
}

// Converts a relative LED index (0-based) within the current half of the keyboard
// (master or slave) to its corresponding absolute LED index
static inline uint8_t to_absolute_led_index(uint8_t led_index) {
    return led_index + FIRST_TOP_LED + (master ? 0 : FIRST_LED_SLAVE);
}

// Horizontal mirror. Converts a led index to the current half of the keyboard
static inline uint8_t mirror_led_index(uint8_t led_index) {
    return led_index + (master ? 0 : FIRST_LED_SLAVE);
}

// Safe version of rgb_matrix_set_color, (not really needed)
static inline void set_led(uint8_t led_index, uint8_t r, uint8_t g, uint8_t b) {
    const uint8_t first = master ? FIRST_TOP_LED : FIRST_TOP_LED_SLAVE;
    const uint8_t last = master ? LAST_TOP_LED : LAST_TOP_LED_SLAVE;
    if (led_index < first || led_index > last) return;
    rgb_matrix_set_color( led_index, safe(r), safe(g), safe(b) );
}

// Safe version of rgb_matrix_set_color for HSV colors
static inline void set_led_hsv(uint8_t led_index, HSV hsv) {
    hsv.v = safe(hsv.v);
    RGB rgb = hsv_to_rgb(hsv);
    set_led(led_index, rgb.r, rgb.g, rgb.b);
}

// Safe RGB led management in ROW (y) COL (x) coordinates. Origin 0,0: top-left
static inline void set_led_2d(uint8_t y, uint8_t x, uint8_t r, uint8_t g, uint8_t b){
    if (x >= NUM_COLUMNS || y >= NUM_ROWS) return;
    if(y == 3 && x < 3) return;
    const uint8_t led = led_map[y][x];
    if (led < 0) return;
    rgb_matrix_set_color( mirror_led_index( led ), safe(r), safe(g), safe(b) );
}

// Safe HSV led management in ROW (y) COL (x) coordinates. Origin 0,0: top-left
static inline void set_led_hsv_2d(uint8_t y, uint8_t x, HSV hsv){
    hsv.v = safe(hsv.v);
    RGB rgb = hsv_to_rgb(hsv);
    set_led_2d( y, x, rgb.r, rgb.g, rgb.b);
}

// Clear animation data 1d array
static inline void clear_buffer_1d(void) {
    memset(buffer_1d, 0, sizeof(buffer_1d));
}

// Clear animation data 2d array
static inline void clear_buffer_2d(void){
    memset(buffer_2d, 0, sizeof(buffer_2d));
}

// Fast integer square root for 32-bit numbers
static uint16_t isqrt(uint32_t n) {
    if (n == 0) return 0;
    uint32_t x = n;
    uint32_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return (uint16_t)x;
}

// Random number in interval
static inline uint8_t random8_interval(uint8_t min, uint8_t max) {
    return min + (((uint16_t)random8() * (max - min + 1)) >> 8);
}

// Multiplies two 16-bit values (a, b) and returns the upper 16 bits of the 32-bit result
static inline uint16_t qmul16(uint16_t a, uint16_t b) {
    return (uint16_t)(((uint32_t)a * (uint32_t)b) >> 16);
}

// Add two 16-bit values with saturation
static inline uint16_t qadd16(uint16_t a, uint16_t b) {
    uint32_t sum = (uint32_t)a + (uint32_t)b;
    return (sum > MAX_INT16) ? MAX_INT16 : (uint16_t)sum;
}

// Saturating subtraction for 16-bit values (no underflow below 0)
static inline uint16_t qsub15(uint16_t value, uint16_t subtract) {
    return (value > subtract) ? (value - subtract) : 0;
}

// Remap a value from one scale to another
static inline uint16_t remap(uint16_t value, uint16_t origin_scale, uint16_t target_scale) {
    return (value * target_scale + origin_scale / 2) / origin_scale;
}

// Generate a palette by interpolating between input colors with ease-in
// to provide greater accuracy in low-brightness LEDs.
// Input example: 3, {0, 50, 100}, {0x000000, 0x0000FF, 0xFFFFFF}
static void generate_palette(uint8_t count, const uint8_t *percents, const uint32_t *colors) {
    if (count < 2 || percents[0] != 0 || percents[count-1] != 100) return;
    // Remap the input colors to the 0-MAX_BRIGHT range
    RGB input_colors[count];
    for (uint8_t i = 0; i < count; i++) {
        const uint32_t c = colors[i];
        const uint8_t r = (c >> 16) & 0xFF;
        const uint8_t g = (c >> 8) & 0xFF;
        const uint8_t b = c & 0xFF;
        input_colors[i].r = (uint8_t)remap(r, MAX_INT8, MAX_BRIGHT);
        input_colors[i].g = (uint8_t)remap(g, MAX_INT8, MAX_BRIGHT);
        input_colors[i].b = (uint8_t)remap(b, MAX_INT8, MAX_BRIGHT);
    }
    const uint16_t slots = PALETTE_LENGTH - 1;
    for (uint8_t i = 0; i < count - 1; i++) {
        // Calculate initial and final indices of the segment
        const uint32_t start_idx = ((uint32_t)percents[i] * slots) / 100;
        const uint32_t end_idx = ((uint32_t)percents[i + 1] * slots) / 100;
        const int32_t den = end_idx - start_idx;
        if (den <= 0) continue; // Avoid division by zero
        // Differences between consecutive colors
        const int32_t delta_r = (int32_t)input_colors[i + 1].r - input_colors[i].r;
        const int32_t delta_g = (int32_t)input_colors[i + 1].g - input_colors[i].g;
        const int32_t delta_b = (int32_t)input_colors[i + 1].b - input_colors[i].b;
        for (uint32_t k = start_idx; k <= end_idx; k++) {
            const int32_t num = k - start_idx;
            // Quadratic interpolation using fixed-point (16 fractional bits)
            const uint32_t num_sq = (uint32_t)num * num;
            const uint32_t den_sq = (uint32_t)den * den;
            const uint32_t t_squared_fp = (num_sq << 16) / den_sq; // t^2 in fixed-point
            // Calculate increments with rounding
            const int32_t increment_r = (delta_r * (int32_t)t_squared_fp + (1 << 15)) >> 16;
            const int32_t increment_g = (delta_g * (int32_t)t_squared_fp + (1 << 15)) >> 16;
            const int32_t increment_b = (delta_b * (int32_t)t_squared_fp + (1 << 15)) >> 16;
            // Final values with clamping
            const int32_t r = (int32_t)input_colors[i].r + increment_r;
            const int32_t g = (int32_t)input_colors[i].g + increment_g;
            const int32_t b = (int32_t)input_colors[i].b + increment_b;
            palette[k].r = (uint8_t)(r < 0 ? 0 : (r > 255 ? 255 : r));
            palette[k].g = (uint8_t)(g < 0 ? 0 : (g > 255 ? 255 : g));
            palette[k].b = (uint8_t)(b < 0 ? 0 : (b > 255 ? 255 : b));
        }
    }
}


//═══════════════════════════════════════════════════════════════
//	DEBUGGING SLAVE
//═══════════════════════════════════════════════════════════════
#ifdef RGB_DEBUG

/*

To decode, use AI:

Three LEDs are arranged from left to right (A, B, and C). Each LED has three channels (R, G, and B) representing bits (0 for off and 1 for on). Thus, each LED encodes 3 bits, forming a 9-bit binary number in total. The bit order is: A: Ar Ag Ab, B: Br Bg Bb, C: Cr Cg Cb.
For example, a white LED (111) has all bits set to 1, while red (100) or purple (101) represent other combinations. By reading the 9 bits from left to right, the corresponding decimal number is obtained. Your task is to return the decimal number given a sequence of 3 colors.

*/

static uint8_t current_row = 0;

// Helper function to encode a number (0–511) into the RGB channels of 3 LEDs (9 bits).
static void set_led_group(uint8_t leds[3], uint16_t number) {
    number &= 0x1FF;
    if (number == 0) {
        for (uint8_t i = 0; i < 3; i++) {
            set_led(mirror_led_index(leds[i]), 1, 1, 1);
        }
    } else {
        for (uint8_t i = 0; i < 3; i++) {
            uint8_t group = (number >> ((2 - i) * 3)) & 0x07;
            uint8_t r = (group & 0x04) ? 100 : 0;
            uint8_t g = (group & 0x02) ? 100 : 0;
            uint8_t b = (group & 0x01) ? 100 : 0;
            set_led(mirror_led_index(leds[i]), r, g, b);
        }
    }
}

// Debug function to visualize two numbers (0–511) on the slave using LEDs
void debug_slave(uint16_t a, uint16_t b) {
    uint8_t row = current_row;
    current_row = (current_row + 1) % 3;
    uint8_t a_leds[3] = {led_map[row][5], led_map[row][4], led_map[row][3]};
    set_led_group(a_leds, a);
    uint8_t b_leds[3] = {led_map[row][2], led_map[row][1], led_map[row][0]};
    set_led_group(b_leds, b);
}

#endif

