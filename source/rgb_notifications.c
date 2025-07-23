//═════════════════════════════════════════════════════════
//   SHORT NOTIFICATION ANIMATIONS
//═════════════════════════════════════════════════════════

// Defines a notification function for dynamic calls.
// Parameters:
//   - hsv: Base or current HSV color. V should be constrained to MAX_BRIGHT.
//   - i: Index of the LED being processed in the matrix.
//   - dx: X-coordinate of the LED for horizontal spatial effects.
//   - dy: Y-coordinate of the LED for vertical spatial effects.
//   - dist: Distance of the LED from (0,0), useful for radial effects.
//   - progress: Animation progress (0-255), not tied to time units but to advancement. Up to 4s at 60fps.
typedef HSV (*NotificationFunction)(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress);
#define NOTIFICATION_FUNCTION(name) static HSV name(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress)

// Notification functions
NOTIFICATION_FUNCTION( windows_notification );
NOTIFICATION_FUNCTION( mac_notification );
NOTIFICATION_FUNCTION( boot_notification );
NOTIFICATION_FUNCTION( lock_notification );
NOTIFICATION_FUNCTION( unlock_notification );
NOTIFICATION_FUNCTION( caps_word_notification );

// Notification functions. Definition order must match 'rgb_notification' enum
static NotificationFunction notification_functions[] = {
    windows_notification,
    mac_notification,
    boot_notification,
    lock_notification,
    unlock_notification,
    caps_word_notification
};

//	NOTIFICATION HANDLING & LIFECYCLE
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Set a notification animation and sync it with the slave (except RGB_NO_NOTIFICATION)
void rgb_set_notification(enum rgb_notification notification){
    reset_time();
    if(current_notification == notification) return;
    current_notification = notification;
    if (notification != RGB_NO_NOTIFICATION) {
        sync(SYNC_RGB_NOTIFICATION, notification, 0);
    }
    dirty();
}

// Draw current notification
static void draw_current_notification(void){
    if( t >= NOTIFICATION_DURATION ){
        rgb_set_notification( RGB_NO_NOTIFICATION );
        return;
    }
    uint8_t progress = (uint8_t) ( t * MAX_INT8 / NOTIFICATION_DURATION );
    for(uint8_t i=FIRST_TOP_LED; i<=LAST_TOP_LED; i++){
        int16_t dx   = g_led_config.point[i].x;
        int16_t dy   = g_led_config.point[i].y;
        uint8_t dist = sqrt16(dx * dx + dy * dy);
        RGB rgb  = hsv_to_rgb( notification_functions[ current_notification ]( rgb_matrix_config.hsv, i, dx, dy, dist, progress ) );
        set_led( mirror_led_index(i),  safe(rgb.r), safe(rgb.g), safe(rgb.b) );
    }
}


//	NOTIFICATION FUNCTIONS  (used for visual notification purposes)
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Windows notification: animated rainbow
static HSV windows_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress) {
    uint8_t valor = cos8( (uint8_t) (dx + ((uint16_t)progress * 4) / 5) );
    hsv.h = valor;
    uint8_t decay = 100;
    if (progress < decay) {
        hsv.v = MAX_BRIGHT;
    } else {
        uint16_t numerator = (uint16_t)(MAX_INT8 - progress) * MAX_BRIGHT;
        uint8_t denominator = MAX_INT8 - decay; // 255 - 100 = 155
        hsv.v = numerator / denominator;
    }
    hsv.s = 255;
    return hsv;
}

// Mac notification: animated white
static HSV mac_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress) {
    hsv.s = scale8(hsv.s + dist - progress - atan2_8(dy, dx), hsv.s);
    hsv.v = MAX_INT8 - progress;
    hsv.h = 150;
    return hsv;
}

// Boot notification: animated red band
static HSV boot_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress) {
    hsv.h = 0;
    hsv.s = MAX_INT8;
    int16_t v = MAX_INT8 - abs(scale8(dx, 140) - ((MAX_INT8 - (((uint16_t)progress * 107) / 100)) / 4)) * 30;
    hsv.v = (v < 0) ? 0 : v;
    return hsv;
}


// Pulsating ring animation of a given color
static HSV pulsating_ring(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t hue, uint8_t progress) {
    int16_t centered_x = dx - CENTER_X;
    int16_t centered_y = dy - CENTER_Y;
    uint8_t distance = sqrt16(centered_x * centered_x + centered_y * centered_y);
    uint8_t max_radius = 70;
    uint8_t pulse_radius = scale8(triwave8(progress), max_radius);
    uint8_t thickness = 15;
    uint8_t delta = abs8(distance - pulse_radius);
    hsv.h = hue;
    hsv.s = 255;
    hsv.v = (delta < thickness) ? MAX_BRIGHT : 0;
    return hsv;
}

// Lock notification: pulsating red ring
static HSV lock_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress) {
    return pulsating_ring(hsv, i, dx, dy, 0, progress);
}

// Lock notification: pulsating green ring
static HSV unlock_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress) {
    return pulsating_ring(hsv, i, dx, dy, 95, progress);
}

// Caps word notification: growing circle with global fade-out
static HSV caps_word_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t progress) {
    hsv.h = 7;
    hsv.s = 255;
    // thumb cluster
    if ((1 << i) & ((1 << 6) | (1 << 13) | (1 << 14))) {
        hsv.v = MAX_BRIGHT;
        return hsv;
    }
    int16_t centered_x = dx - THUMB_X;
    int16_t centered_y = dy - THUMB_Y;
    uint8_t distance = sqrt16(centered_x * centered_x + centered_y * centered_y);
    uint8_t max_radius = 255;
    uint8_t r = scale8(progress+10, max_radius);

    if (r == 0) {
        hsv.v = (distance == 0) ? MAX_BRIGHT : 0;
    } else if (distance > r) {
        hsv.v = 0;
    } else {
        uint16_t temp = (255 - progress) * MAX_BRIGHT;
        uint16_t inner_v = ((uint16_t)distance * MAX_BRIGHT) / r;
        temp += progress * inner_v;
        hsv.v = (temp >> 8);
    }
    uint8_t fade_start_time = 190;
    if (progress > fade_start_time) {
        uint8_t fade_duration = 255 - fade_start_time; // 65 en este caso
        uint8_t fade_progress = progress - fade_start_time;
        uint8_t fade_factor = scale8(255 - (fade_progress * 255 / fade_duration), 255);
        hsv.v = scale8(hsv.v, fade_factor);
    }
    return hsv;
}
