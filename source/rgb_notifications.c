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
//   - time: Animation progress (0-255), not tied to time units but to advancement. Up to 4s at 60fps.
typedef HSV (*NotificationFunction)(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);

// Notification functions
static HSV windows_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);
static HSV mac_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);
static HSV lock_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);
static HSV ring_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);
static HSV caps_word_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);

// Notification functions. Definition order must match 'rgb_notification' enum
static NotificationFunction notification_functions[] = {
    windows_notification,
    mac_notification,
    lock_notification,
    ring_notification,
    caps_word_notification
};


//	NOTIFICATION HANDLING & LIFECYCLE
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// this allows to prevent sync
void rgb_set_notification_internal(enum rgb_notification notification, bool sync_enable) {
    reset_time();
    current_notification = notification;
    if (sync_enable) {
        sync(SYNC_RGB_NOTIFICATION, notification, 0);
    }
    dirty();
}

// Set a notification animation and sync it with the slave
void rgb_set_notification(enum rgb_notification notification){
    rgb_set_notification_internal(notification, true);
}

// Draw current notification
static void draw_current_notification(void){
    elapsed += dt;
    if(elapsed >= NOTIFICATION_DURATION){
        rgb_set_notification_internal(RGB_NO_NOTIFICATION, false);
        return;
    }
    uint8_t time = (uint8_t) ((uint32_t)elapsed * MAX_INT8 / NOTIFICATION_DURATION);
    for(uint8_t i=FIRST_TOP_LED; i<=LAST_TOP_LED; i++){
        int16_t dx   = g_led_config.point[i].x;
        int16_t dy   = g_led_config.point[i].y;
        uint8_t dist = sqrt16(dx * dx + dy * dy);
        RGB rgb  = hsv_to_rgb( notification_functions[ current_notification ]( rgb_matrix_config.hsv, i, dx, dy, dist, time ) );
        set_led( mirror_led_index(i),  safe(rgb.r), safe(rgb.g), safe(rgb.b) );
    }
}


//	NOTIFICATION FUNCTIONS  (used for visual notification purposes)
//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Windows notification: animated rainbow
static HSV windows_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    uint8_t valor = cos8( (uint8_t) (dx + ((uint16_t)time * 4) / 5) );
    hsv.h = valor;
    uint8_t decay = 100;
    if (time < decay) {
        hsv.v = MAX_BRIGHT;
    } else {
        uint16_t numerator = (uint16_t)(MAX_INT8 - time) * MAX_BRIGHT;
        uint8_t denominator = MAX_INT8 - decay; // 255 - 100 = 155
        hsv.v = numerator / denominator;
    }
    hsv.s = 255;
    return hsv;
}

// Mac notification: animated white
static HSV mac_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    hsv.s = scale8(hsv.s + dist - time - atan2_8(dy, dx), hsv.s);
    hsv.v = MAX_INT8 - time;
    hsv.h = 150;
    return hsv;
}

// Lock notification: animated red band
static HSV lock_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    hsv.h = 0;
    hsv.s = MAX_INT8;
    int16_t v = MAX_INT8 - abs(scale8(dx, 140) - ((MAX_INT8 - (((uint16_t)time * 107) / 100)) / 4)) * 30;
    hsv.v = (v < 0) ? 0 : v;
    return hsv;
}

// Ring notification: pulsating ring
static HSV ring_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    int16_t centered_x = dx - CENTER_X;
    int16_t centered_y = dy - CENTER_Y;
    uint8_t distance = sqrt16(centered_x * centered_x + centered_y * centered_y);
    uint8_t max_radius = 70;
    uint8_t pulse_radius = scale8(triwave8(time), max_radius);
    uint8_t thickness = 15;
    uint8_t delta = abs8(distance - pulse_radius);
    hsv.h = 64;
    hsv.s = 255;
    hsv.v = (delta < thickness) ? MAX_BRIGHT : 0;
    return hsv;
}

// Caps word notification: growing circle with global fade-out
static HSV caps_word_notification(HSV hsv, uint8_t i, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
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
    uint8_t r = scale8(time+10, max_radius);

    if (r == 0) {
        hsv.v = (distance == 0) ? MAX_BRIGHT : 0;
    } else if (distance > r) {
        hsv.v = 0;
    } else {
        uint16_t temp = (255 - time) * MAX_BRIGHT;
        uint16_t inner_v = ((uint16_t)distance * MAX_BRIGHT) / r;
        temp += time * inner_v;
        hsv.v = (temp >> 8);
    }
    uint8_t fade_start_time = 190;
    if (time > fade_start_time) {
        uint8_t fade_duration = 255 - fade_start_time; // 65 en este caso
        uint8_t fade_progress = time - fade_start_time;
        uint8_t fade_factor = scale8(255 - (fade_progress * 255 / fade_duration), 255);
        hsv.v = scale8(hsv.v, fade_factor);
    }
    return hsv;
}
