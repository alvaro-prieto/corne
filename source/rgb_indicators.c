//═══════════════════════════════════════════════════════════════
//	RGB INDICATORS (keyboard state visualizations)
//═══════════════════════════════════════════════════════════════

// Used as node in a notification led pattern. It contains its location,
// color, and next LED node in the pattern
typedef struct {
    uint8_t number; // Greater than 100 (slave). 200 in both sides. 255 thumb cluster
    HSV color;
    uint8_t next;
} __attribute__((packed)) led_group;


// The first value corresponds to the LED positioning as depicted in the illustration (rgb.c)
// The second value is the color in HSV. V is constrained to MAX_BRIGHT
// The third value is the next LED in the group, by its index in the leds[] array
// Position < 100 = master, >= 100 && < 200 = slave, >= 200 = both, 255 = thumb cluster
static const led_group leds[] PROGMEM = {
    // 0 - RGB_HYPER
    {126, {231, 255, safe(255)}, 0},
    // 1 - RGB_CAPS_LOCK
    {126, {125, 215, safe(209)}, 2},
    {255, {125, 215, safe(209)}, 0},
    // 3 - RGB_CAPS_WORD
    {255, {7, 255, safe(187)}, 0},
    // 4 - RGB_EXTRAS
    {255, {0, 0, safe(255)}, 0},
    // 5 - RGB_HYPER_LOCK
    {255, {231, 255, safe(255)}, 6},
    {126, {221, 255, safe(54)}, 0},
    // 7 - RGB_NUM_LOCK
    {255, {177, 250, safe(163)}, 8},
    {26,  {177, 250, safe(163)}, 0}
};

// Draws an LED based on its position (see illustration above)
// value < 100  = left half
// value >= 100 && < 200 = right half
// value >= 200 = both sides
static void draw_single_led(uint8_t index, HSV color){
    const uint8_t adjust = index >= 200 ? 200 : index >= 100 ? 100 : 0;
    if( (master &&  (index < 100 || index >= 200) ) ||
        (!master && index >= 100)){
        index += master ? 0 : FIRST_LED_SLAVE;
        set_led_hsv( index - adjust, color);
    }
}

// Draw an indicator, that can be compound of several LEDs
static void draw_indicator(enum rgb_indicator indicator) {
    led_group led;
    for (uint8_t i = 0; i < MAX_INDICATOR_LENGTH; i++) {
        memcpy_P(&led, &leds[indicator], sizeof(led_group));
        uint8_t index = led.number;
        HSV color = led.color;
        if (index == 255) {
            draw_single_led(206, color);
            draw_single_led(213, color);
            draw_single_led(214, color);
        } else {
            draw_single_led(index, color);
        }
        if (led.next == 0) break;
        indicator = led.next;
    }
}

// Draw all active indicators
static void draw_indicators(void){
    for (uint8_t i = 0; i < number_of_active_indicators; i++) {
        draw_indicator(active_indicators[i]);
    }
}

// Remove all active indicators
void reset_indicators(void){
    number_of_active_indicators = 0;
}

// Function that can be called from other files to enable a visual indicator,
// such as caps lock, caps word, hyper, ...
void rgb_set_indicator(enum rgb_indicator indicator, bool on) {
    if (on) {
        for (uint8_t i = 0; i < number_of_active_indicators; i++)
            if (active_indicators[i] == indicator) return;
        if (number_of_active_indicators < MAX_NUMBER_OF_INDICATORS) {
            active_indicators[number_of_active_indicators++] = indicator;
            sync(SYNC_RGB_INDICATOR, indicator, true);
            dirty();
        }
    } else {
        for (uint8_t i = number_of_active_indicators; i-- > 0; ) {
            if (active_indicators[i] == indicator) {
                active_indicators[i] = active_indicators[--number_of_active_indicators];
                sync(SYNC_RGB_INDICATOR, indicator, false);
                dirty();
                break;
            }
        }
    }
}
