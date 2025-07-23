
//═══════════════════════════════════════════════════════════════
//	NON-REACTIVE RGB ANIMATIONS
//═══════════════════════════════════════════════════════════════

// Animation functions
#include "rgb.h"
static void lava_init(void);
static RGB  lava_render_led(uint8_t i, int16_t dx, int16_t dy);
static void plasma_init(void);
static RGB  plasma_render_led(uint8_t i, int16_t dx, int16_t dy);
static void the_matrix_init(void);
static void the_matrix_render_frame(void);
static void fire_init(void);
static void fire_render_frame(void);
static void boreal_init(void);
static void boreal_render_frame(void);

// Animation mapping
static struct {
    void (*init)(void);  // Init function, called once when the animation is set
    void (*render_frame)(void);  // Frame render function, called once per frame
    RGB (*render_led)(uint8_t i, int16_t dx, int16_t dy);  // LED render function
} animations[] = {
    [RGB_LAVA_ANIMATION]    = { lava_init, NULL, lava_render_led },
    [RGB_PLASMA_ANIMATION]  = { plasma_init, NULL, plasma_render_led },
    [RGB_MATRIX_ANIMATION]  = { the_matrix_init, the_matrix_render_frame, NULL },
    [RGB_FIRE_ANIMATION]    = { fire_init, fire_render_frame, NULL },
    [RGB_BOREAL_ANIMATION]  = { boreal_init, boreal_render_frame, NULL },
    [RGB_NO_ANIMATION]      = { NULL, NULL, NULL }
};

// Function to set the current animation
void rgb_set_animation(enum rgb_animation animation) {
    rgb_disable_all_except( RGB_ANIMATION );
    current_animation = animation;
    reset_time();
    if (animation != RGB_NO_ANIMATION) {
        if (animations[animation].init != NULL) {
            animations[animation].init();
        }
    }
    dirty();
    sync(SYNC_RGB_ANIMATION, animation, 0);
}

// Function to rotate between animations
void rgb_rotate_animation(int8_t direction) {
    current_animation = (current_animation + direction + RGB_NO_ANIMATION + 1) % (RGB_NO_ANIMATION + 1);
    rgb_set_animation(current_animation);
}

// Function to draw the current animation
static void draw_current_animation(void) {
    if (current_animation == RGB_NO_ANIMATION) return;
    if (animations[current_animation].render_frame != NULL) {
        animations[current_animation].render_frame();
    }
    if (animations[current_animation].render_led != NULL) {
        for (uint8_t i = FIRST_TOP_LED; i <= LAST_TOP_LED; i++) {
            uint8_t dx = g_led_config.point[i].x;
            uint8_t dy = g_led_config.point[i].y;
            RGB rgb = animations[current_animation].render_led(i, dx, dy);
            set_led(mirror_led_index(i), safe(rgb.r), safe(rgb.g), safe(rgb.b));
        }
    }
}

//––––––––––––––––––––––––––––––––––––––––––––––––––
// Lava effect 🌋
static void lava_init(void) {
    uint8_t percents[] = {0, 20, 40, 60, 80, 100};
    uint32_t colors[] = {
        0x000000,
        0xFF0000,
        0xFF3300,
        0xFFFF00,
        0xFFFFFF,
        0xFFFFFF
    };
    generate_palette(6, percents, colors);
}

static RGB lava_render_led(uint8_t i, int16_t dx, int16_t dy) {
    // uint8_t noise = sin8(t / 20 + i * 10); // left to right
    uint8_t noise = sin8(t / 20 + (LAST_TOP_LED - i) * 10); //right to left
    uint8_t intensity = scale8(255 - (dy + 32), 255);
    uint8_t value = scale8(noise, intensity);
    uint16_t index = remap(value, 255, PALETTE_LENGTH - 1);
    return palette[index];
}

//––––––––––––––––––––––––––––––––––––––––––––––––––
// Plasma effect 🏳️‍🌈
static void plasma_init(void) {
    uint8_t percents[] = {0, 40, 50, 70, 100};
    uint32_t colors[] = {
        0xFF0000,
        0xFFFF00,
        0x00FF00,
        0x0000FF,
        0xFF00FF
    };
    generate_palette(5, percents, colors);
}

static RGB plasma_render_led(uint8_t i, int16_t dx, int16_t dy) {
    uint8_t sin1 = sin8(dx * 2 + t / 10);      // X wave, slow shift
    uint8_t sin2 = sin8(dy * 3 + t / 15);      // Y wave, medium shift
    uint8_t sin3 = sin8((dx + dy) + t / 20);   // Diagonal wave, fast shift
    uint16_t value = (sin1 + sin2 + sin3);     // Average, range 0-255*3
    uint16_t index = remap(value, MAX_INT8*3, PALETTE_LENGTH - 1);
    return palette[index];
}


//––––––––––––––––––––––––––––––––––––––––––––––––––
// The Matrix effect 🈯️
// Implemented with two types of drop trails: linear and cubic.
// The type value is stored in the least significant positions of the data buffer.
// Drop row (0->NUM_ROWS; MAX_INT16 = empty), to track the position of the drop, stored in buffer_1d: 0 -> NUM_COLUMNS-1
// Drop type (0 = normal, 1 = cubic) sored in buffer_1d: NUM_COLUMNS -> NUM_COLUMNS*2-1

static void the_matrix_init(void) {
    uint8_t percents[] = {0, 8, 18, 35, 60, 100};
    uint32_t colors[] = {
        0xFFFFFF,0x00FF00, 0x00AA00, 0x004400, 0x001100, 0x000000
    };
    generate_palette(6, percents, colors);
    clear_buffer_1d();
    clear_buffer_2d();
    for (uint8_t c = 0; c < NUM_COLUMNS; c++) {
        buffer_1d[c] = MAX_INT16; // Drop row (empty)
        if (random8() < 30){
            buffer_1d[c] = 0;
            buffer_1d[c + NUM_COLUMNS] = (random8() < 51) ? 1 : 0; // Drop type, 20% chance of cubic type
        }
    }
}

static void the_matrix_render_frame(void) {
    static uint8_t frame_counter = 0;
    for (uint8_t i = 0; i < NUM_ROWS; i++) {
        for (uint8_t j = 0; j < NUM_COLUMNS; j++) {
            buffer_2d[i][j] = qsub15(buffer_2d[i][j], 0x300); // Fade out
        }
    }
    // Move drops down & seed new ones
    if ( !(frame_counter++ % 4) ) {
        for (uint8_t c = 0; c < NUM_COLUMNS; c++) {
            if (buffer_1d[c] != MAX_INT16) {
                if (++buffer_1d[c] >= NUM_ROWS) {
                    buffer_1d[c] = MAX_INT16;
                }
            } else if (random8() < 7) {  // new seed
                buffer_1d[c] = 0;
                buffer_1d[c + NUM_COLUMNS] = (random8() < 51) ? 1 : 0; // 20% chance of cubic type
            }
        }
    }
    for (uint8_t c = 0; c < NUM_COLUMNS; c++) {
        if (buffer_1d[c] == -1) continue;
        uint8_t max_row = MIN(buffer_1d[c], NUM_ROWS - 1);
        for (int8_t r = max_row; r >= 0; r--) {
            uint8_t distance = buffer_1d[c] - r;
            uint16_t intensity = 255 - (distance * 60);  // fade
            if (intensity > 255) intensity = 0;
            // type is stored in the least significant bits of the trail
            buffer_2d[r][c] = MAX(buffer_2d[r][c], (intensity << 8) | buffer_1d[c + NUM_COLUMNS]);
        }
    }
    // Render
    for (uint8_t c = 0; c < NUM_COLUMNS; c++) {
        for (uint8_t r = 0; r < NUM_ROWS; r++) {
            // Two values are stored in the data buffer
            uint8_t value = buffer_2d[r][c] >> 8;
            uint8_t type = buffer_2d[r][c] & 0xFF;
            uint16_t palette_index;
            if (type) {
                palette_index = remap(255 - cubicwave8(value), 255, PALETTE_LENGTH - 1);
            } else {
                palette_index = remap(255 - value, 255, PALETTE_LENGTH - 1);
            }
            RGB rgb = palette[palette_index];
            set_led_2d( r, c, rgb.r, rgb.g, rgb.b);
        }
    }
}

//––––––––––––––––––––––––––––––––––––––––––––––––––
// Fire effect 🔥
static void fire_init(void) {
    uint8_t percents[] = {0, 10, 25, 40, 60, 80, 95, 100};
    uint32_t colors[] = {
        0x000000, 0x220000, 0x882000, 0xFF4000,
        0xFF8000, 0xFFC000, 0xFFFF40, 0xFFFFFF
    };
    generate_palette(8, percents, colors);
    clear_buffer_1d();
}

static void fire_render_frame(void) {
    for (uint8_t row = 0; row < NUM_ROWS - 1; row++) {
        for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
            // Average of the bottom row neighbors
            uint16_t left = buffer_2d[row + 1][(col - 1 + NUM_COLUMNS) % NUM_COLUMNS];
            uint16_t center = buffer_2d[row + 1][col];
            uint16_t right = buffer_2d[row + 1][(col + 1) % NUM_COLUMNS];
            uint16_t propagated = (left + center + right) / 3;
            // Linear decay based on height (higher = colder)
            uint16_t decay = (uint16_t)( (row+1) * 5000);
            // uint16_t decay = (row + 1) * (3500 + ((sin8(col*40 + t/30) * 1900) >> 8));
            uint16_t base_heat = propagated > decay ? propagated - decay : 0;
            buffer_2d[row][col] = base_heat ;
        }
    }
    // Generation of heat seeds
    for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
        if (random8() < 20) {  // Chances of being hot
            buffer_2d[NUM_ROWS - 1][col] = (uint16_t)(0x8000 + (random8() * 0x200)); // 0x80 - overflow is intended
        } else {    // Soft decay when no flame is emitted
            buffer_2d[NUM_ROWS - 1][col] = (buffer_2d[NUM_ROWS - 1][col] > 0x200) ? (buffer_2d[NUM_ROWS - 1][col] - 0x200) : 0;
        }
    }
    // Render
    for (uint8_t row = 0; row < NUM_ROWS-1; row++) {
        for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
            uint16_t p = remap( buffer_2d[row][col] , MAX_INT16, PALETTE_LENGTH - 1);
            uint16_t avg = (p + buffer_1d[row*NUM_COLUMNS + col]*5) / 6;
            buffer_1d[row*NUM_COLUMNS + col] = avg;
            set_led_2d(row, col, palette[avg].r, palette[avg].g, palette[avg].b);
        }
    }
    // Thumb area custom colour
    uint16_t col[] = {400, 470, 511};
    for (uint8_t thumb = 0; thumb < 3; thumb++) {
        uint16_t p = remap( buffer_2d[3][thumb+3] , MAX_INT16, PALETTE_LENGTH - 1);
        p = (p + col[thumb])/2;
        const uint8_t led = 3*NUM_COLUMNS + thumb+3;
        uint16_t avg = (p + buffer_1d[led]*7) / 8 ;
        buffer_1d[led] = avg;
        set_led_2d( 3, thumb + 3, palette[avg].r, palette[avg].g, palette[avg].b );
    }
}

//––––––––––––––––––––––––––––––––––––––––––––––––––
// Aurora boreal effect 🌌
static void boreal_init(void) {
    uint8_t percents[] = {0, 15, 30, 45, 55, 70, 85, 100};
    uint32_t colors[] = {
        0x000000,
        0x000033,
        0x003399,
        0x0088BB,
        0x000088,
        0x220044,
        0x000033,
        0x000000
    };
    generate_palette(8, percents, colors);
    clear_buffer_2d();
}

static void boreal_render_frame(void) {
    static uint16_t t = 0;
    t += 10;
    for (uint8_t y = 0; y < NUM_ROWS; y++) {
        for (uint8_t x = 0; x < NUM_COLUMNS; x++) {
            uint8_t wave1 = sin8(x * 17 + (t >> 4));
            uint8_t wave2 = sin8(y * 23 + (t >> 5));
            uint8_t wave3 = cubicwave8((x + y) * 25 + (t >> 4));
            uint8_t wave4 = sin8((x - y) * 27 + (t >> 6));
            uint16_t sum = wave1 + wave2 + wave3 + wave4;
            uint16_t idx = remap(sum, 1020, PALETTE_LENGTH);
            idx = (idx + buffer_2d[y][x]*7) / 8;
            // Smooth average with previous frame
            RGB color = palette[(idx + (t >> 2)) % PALETTE_LENGTH];
            buffer_2d[y][x] = idx;
            set_led_2d(y, x, color.r, color.g, color.b);
        }
    }
}
