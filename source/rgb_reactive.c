#include <stdint.h>
#include "rgb.h"
#include "sync.h"

//═════════════════════════════════════════════════════════
//   REACTIVE RGB ANIMATIONS
//═════════════════════════════════════════════════════════


// Reactive animation headers
static void heatmap_init(void);
static void heatmap_animate(void);
static void heatmap_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down);
static void heatmap_long_init(void);
static void heatmap_long_animate(void);
static void heatmap_long_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down);
static void diffusion_init(void);
static void diffusion_fire_init(void);
static void diffusion_animate(void);
static void diffusion_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down);
static void magic_init(void);
static void magic_animate(void);
static void magic_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down);
static void ring_wave_init(void);
static void ring_wave_animate(void);
static void ring_wave_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down);

// Reactive animation mapping
static struct {
    void (*init)(void);     // Init function
    void (*animate)(void);  // Animation function
    void (*keypress)(uint8_t, uint8_t, uint8_t, bool); // Keypress function
} reactive_animations[] = {
    [RGB_REACTIVE_DIFFUSION] = { diffusion_init, diffusion_animate, diffusion_keypress },
    [RGB_REACTIVE_DIFFUSION_FIRE] ={ diffusion_fire_init, diffusion_animate, diffusion_keypress },
    [RGB_REACTIVE_HEATMAP] = { heatmap_init, heatmap_animate, heatmap_keypress },
    [RGB_REACTIVE_LONG_HEATMAP] = { heatmap_long_init, heatmap_long_animate, heatmap_long_keypress },
    [RGB_REACTIVE_MAGIC] = { magic_init, magic_animate, magic_keypress },
    [RGB_REACTIVE_RING_WAVE]   = { ring_wave_init, ring_wave_animate, ring_wave_keypress },
    [RGB_NO_REACTIVE] = { NULL, NULL, NULL}
};

// Function that can be called from other files to change the current
// reactive animation. Direction can be either 1 or -1
void rgb_rotate_reactive( int8_t direction ){
    current_reactive = (current_reactive + direction + RGB_NO_REACTIVE + 1) % (RGB_NO_REACTIVE + 1);
    rgb_set_reactive(current_reactive);
}

// Function to set the current reactive animation
void rgb_set_reactive(enum rgb_reactive reactive) {
    rgb_disable_all_except( RGB_REACTIVE );
    current_reactive = reactive;
    reset_time();
    if (reactive_animations[ current_reactive ].init) {
        reactive_animations[ current_reactive ].init();
    }
    dirty();
    rgb_update_reactiveness();
    sync(SYNC_RGB_REACTIVE, reactive, 0);
}

// Fired when a key is pressed and reactive animations are enabled
void rgb_keyboard_event(uint8_t col, uint8_t row, bool down){
    bool slave_event = row > 3;
    if (!(master ^ slave_event)) return;
    uint8_t led_index = MAX_INT8;
    bool found = rgb_matrix_map_row_column_to_led(row, col, &led_index);
    led_index = to_relative_led_index( led_index );
    if(!found || led_index >= TOP_LED_COUNT_HALF || led_index < 0) return;
    void (*keypress_function)(uint8_t, uint8_t, uint8_t, bool) = reactive_animations[current_reactive].keypress;
    if (keypress_function != NULL) {
        row = master ? row : row - NUM_ROWS;
        keypress_function(led_index, col, row, down);
    }
}

// Called in each frame if a reactive animation is enabled
static void draw_current_reactive_animation(void){
    void (*draw_function)(void) = reactive_animations[current_reactive].animate;
    if (draw_function != NULL) {
        draw_function();
    }
}


//═══════════════════════════════════════════════════════════════
//	REACTIVE ANIMATION FUNCTIONS
//═══════════════════════════════════════════════════════════════

// Heatmap animation
//––––––––––––––––––––––––––––––––––––––––––––––––––
static void heatmap_init(void) {
    uint8_t percents[] = {0, 20, 70, 90, 100};
    uint32_t colors[] = {
        0x000000,  // #000000
        0x000055,  // #000055
        0x0000FF,  // #0000FF
        0x00FFFF,  // #00FFFF
        0xFFFFFF   // #FFFFFF
    };
    generate_palette(5, percents, colors);
}
static void heatmap_animate( void ){
    uint32_t decay = (uint32_t)((uint64_t)MAX_INT16 * dt / 10000);
    for (int i = 0; i < TOP_LED_COUNT_HALF; i++) {
        buffer_1d[i] = (buffer_1d[i] > decay) ? buffer_1d[i] - decay : 0;
        RGB color = palette[ remap( buffer_1d[i], MAX_INT16, PALETTE_LENGTH -1) ];
        set_led( to_absolute_led_index(i) , color.r, color.g, color.b);
    }
}
static void heatmap_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down) {
    buffer_1d[ led_index ] = qadd16(buffer_1d[ led_index ], 8000);
}

// Long heatmap animation
//––––––––––––––––––––––––––––––––––––––––––––––––––
static void heatmap_long_init(void) {
    uint8_t percents[] = {0, 20, 35, 55, 75, 90, 100};
    uint32_t colors[] = {
        0x000000,  // #000000
        0x0000FF,  // #000088
        0x660088,  // #660088
        0xFF0000,  // #FF0000
        0xFF7B00,  // #ff7b00
        0xFFEE00,  // #ffee00
        0xFFFFFF   // #ffffff
    };
    generate_palette(7, percents, colors);
}
static void heatmap_long_animate( void ){
    const uint32_t ms = 60000;
    uint32_t decay = ( (uint64_t)MAX_INT16 * dt + ms - 1 ) / ms;
    for (int i = 0; i < TOP_LED_COUNT_HALF; i++) {
        buffer_1d[i] = (buffer_1d[i] > decay) ? buffer_1d[i] - decay : 0;
        RGB color = palette[ remap( buffer_1d[i], MAX_INT16, PALETTE_LENGTH -1) ];
        set_led( to_absolute_led_index(i), color.r, color.g, color.b);
    }
}
static void heatmap_long_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down) {
    buffer_1d[ led_index ] = qadd16(buffer_1d[ led_index ], 3000);
}


// Diffusion animation
//––––––––––––––––––––––––––––––––––––––––––––––––––
static void diffusion_init(void) {
    uint8_t percents[] = {0, 10, 30, 60, 100};
    uint32_t colors[] = {
        0x000000,   // #000000
        0x000055,   // #000055
        0x0000FF,   // #0000FF
        0x00FFFF,   // #00FFFF
        0xFFFFFF    // #FFFFFF
    };
    generate_palette(5, percents, colors);
    clear_buffer_2d();
    clear_buffer_1d();
}
static void diffusion_fire_init(void) {
    uint8_t percents[] = {0, 10, 25, 40, 60, 80, 95, 100};
    uint32_t colors[] = {
        0x000000, 0x220000, 0x882000, 0xFF4000,
        0xFF8000, 0xFFC000, 0xFFFF40, 0xFFFFFF
    };
    generate_palette(8, percents, colors);
    clear_buffer_2d();
    clear_buffer_1d();
}
static void diffusion_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down) {
    if(!down) return;
    buffer_2d[row][col] = MAX_INT16;
}
static void diffusion_animate(void) {
    static const uint8_t SHIFT_DIFFUSION = 5;   // Higher value = slower diffusion
    static const uint8_t SHIFT_DECAY = 3;       // Higher value = slower decay
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            int32_t sum = 0;
            int32_t current = buffer_2d[i][j];
            int num_neighbors = 0;
            if (i > 0) { sum += buffer_2d[i - 1][j]; num_neighbors++; }
            if (i < NUM_ROWS - 1) { sum += buffer_2d[i + 1][j]; num_neighbors++; }
            if (j > 0) { sum += buffer_2d[i][j - 1]; num_neighbors++; }
            if (j < NUM_COLUMNS - 1) { sum += buffer_2d[i][j + 1]; num_neighbors++; }

            // Compute Laplacian (difference between the average of neighbors and the current value)
            int32_t laplacian = sum - num_neighbors * current;
            int32_t new_intensity = current + (laplacian >> SHIFT_DIFFUSION);
            new_intensity -= new_intensity >> SHIFT_DECAY; // Decay
            if( current > 30000 ){ new_intensity = (new_intensity + current*3) / 4;}
            if (new_intensity < 0) new_intensity = 0;
            else if (new_intensity > MAX_INT16) new_intensity = MAX_INT16;
            // Store new intensity in a temporal buffer
            buffer_1d[i * NUM_COLUMNS + j] = (uint16_t)new_intensity;
        }
    }
    // Consolidate the new state
    memcpy(buffer_2d, buffer_1d, sizeof(buffer_2d));
    // Render
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            uint16_t intensity = buffer_2d[i][j];
            uint16_t index = remap(intensity, MAX_INT16, PALETTE_LENGTH - 1);
            RGB color = palette[index];
            set_led_2d(i, j, color.r, color.g, color.b);
        }
    }
}


//––––––––––––––––––––––––––––––––––––––––––––––––––
// Magic effect 🪄
static void magic_init(void) {
    uint8_t percents[] = {0, 18, 30, 50, 70, 100};
    uint32_t colors[] = {
        0x000000, // #000000,
        0x001f9c, // #001f9c,
        0x87044c, // #87044c,
        0xf60098, // #f60098,
        0xfa44a8, // #fa44a8,
        0xfa44a8  // #fa44a8,
    };
    generate_palette(6, percents, colors);
    buffer_small[0] = 0; // used to track the WPM
    clear_buffer_1d();
}
static void magic_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down) {
    uint16_t intensity = buffer_small[0] + 1000;
    intensity = intensity > 8000 ? 8000 : intensity;
    buffer_small[0] =  intensity;
}
static void magic_animate(void) {
    for (uint8_t row = 0; row < NUM_ROWS - 1; row++) {
        for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
            // Average of the bottom row neighbors
            uint16_t left = buffer_2d[row + 1][(col - 1 + NUM_COLUMNS) % NUM_COLUMNS];
            uint16_t center = buffer_2d[row + 1][col];
            uint16_t right = buffer_2d[row + 1][(col + 1) % NUM_COLUMNS];
            uint16_t propagated = (left + center + right) / 3;
            uint16_t decay = ( (NUM_ROWS - row) * (3500 + ((sin8(col*40 + t/30) * 1900) >> 8)) );
            uint16_t base_heat = propagated > decay ? propagated - decay : 0;
            buffer_2d[row][col] = base_heat ;
        }
    }
    // Generation of heat seeds
    for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
        if (random8() < 20) {  // Chances of being hot
            uint16_t max_value = (uint16_t)(0x8000 + (random8() * 0x200)); // 0x80 - overflow is intended
            uint16_t interpolated_value = 0;
            if (buffer_small[0] < 500) {
                interpolated_value = 0;
            } else if (buffer_small[0] >= 7000) {
                interpolated_value = max_value;
            } else {
                interpolated_value = ((buffer_small[0] - 500) * max_value) / 6500;
            }
            buffer_2d[NUM_ROWS - 1][col] = interpolated_value;
        } else {    // Soft decay when no flame is emitted
            buffer_2d[NUM_ROWS - 1][col] = (buffer_2d[NUM_ROWS - 1][col] > 0x200) ? (buffer_2d[NUM_ROWS - 1][col] - 0x200) : 0;
        }
    }

    buffer_small[0] = buffer_small[0] > 30 ? buffer_small[0] - 30 : 0;

    // Render
    for (uint8_t row = 0; row < NUM_ROWS; row++) {
        for (uint8_t col = 0; col < NUM_COLUMNS; col++) {
            uint16_t p = remap( buffer_2d[row][col] , MAX_INT16, PALETTE_LENGTH - 1);
            uint16_t avg = (p + buffer_1d[row*NUM_COLUMNS + col]*5) / 6;
            buffer_1d[row*NUM_COLUMNS + col] = avg;
            set_led_2d(row, col, palette[avg].r, palette[avg].g, palette[avg].b);
        }
    }
}

// Ring wave animation 🌀
//––––––––––––––––––––––––––––––––––––––––––––––––––
//  An expanding, anti-aliased ring emanates from each keypress.

// Slot Index: stored in buffer_small[0]
// For each wave:
// buffer_1d[i*3 + 0]: (col << 8) | row      (origin coordinates, col and row)
// Since t is 32-bit, we split it into two 16-bit parts
// buffer_1d[i*3 + 1]: (uint16_t)(start_time >> 16) (t high bits)
// buffer_1d[i*3 + 2]: (uint16_t)(start_time & 0xFFFF) (t low bits)
// NOTE: The size of global `buffer_1d` must be at least `MAX_WAVES * 3`.

static void ring_wave_init(void) {
    uint8_t percents[] = {0, 50, 80, 100};
    uint32_t colors[] = {
        0x000000,  // #000000
        0x000043,  // #000043
        0x004455,  // #004455
        0x18636a   // #18636a
    };
    generate_palette(4, percents, colors);
    clear_buffer_1d();
    buffer_small[0] = 0;
}

static void ring_wave_keypress(uint8_t led_index, uint8_t col, uint8_t row, bool down) {
    if (!down) return;
    // Max simultaneous waves. This must match the loop in `ring_wave_animate`
    // and correspond to the allocated size of `buffer_1d`.
    const uint8_t MAX_WAVES = 8;
    // Use buffer_small[0] as our round-robin index.
    uint8_t slot = buffer_small[0];
    // Store wave data in the current slot
    buffer_1d[slot * 3 + 0] = (col << 8) | row;
    buffer_1d[slot * 3 + 1] = (uint16_t)(t >> 16);
    buffer_1d[slot * 3 + 2] = (uint16_t)(t & 0xFFFF);
    // Advance the index for the next keypress
    buffer_small[0] = (slot + 1) % MAX_WAVES;
}

static void ring_wave_animate(void) {
    const uint8_t MAX_WAVES = 8;
    const uint16_t WAVE_LIFESPAN = 2000; // Wave lifespan in ms
    const uint16_t WAVE_SPEED = 200;
    const uint16_t WAVE_THICKNESS = 650;
    const uint16_t half_thickness = WAVE_THICKNESS / 2;
    const uint8_t INTENSITY_FADEOFF = 200; // 0 = no fade. 255 = fades to black over its lifespan
    // Clear the render buffer for this frame.
    clear_buffer_2d();

    // 1. Calculate intensity contributions from all active waves
    for (uint8_t i = 0; i < MAX_WAVES; ++i) {
        uint32_t start_time = ((uint32_t)buffer_1d[i * 3 + 1] << 16) | buffer_1d[i * 3 + 2];
        if (start_time == 0) continue; // Inactive slot
        uint32_t age = t - start_time;
        if (age > WAVE_LIFESPAN) {
            // Kill old wave by clearing its timestamp
            buffer_1d[i * 3 + 1] = 0;
            buffer_1d[i * 3 + 2] = 0;
            continue;
        }
        uint16_t packed_coords = buffer_1d[i * 3 + 0];
        int16_t wave_c = packed_coords >> 8;
        int16_t wave_r = packed_coords & 0xFF;
        // Radius grows with age. Using fixed point (<<8) for precision.
        uint32_t radius = (age * WAVE_SPEED) >> 8;
        // Iterate over every LED to calculate its intensity
        for (int8_t r = 0; r < NUM_ROWS; ++r) {
            for (int8_t c = 0; c < NUM_COLUMNS; ++c) {
                // Calculate distance from LED to wave center.
                int32_t dc = c - wave_c;
                int32_t dr = r - wave_r;
                // Use fixed point (<<16) before sqrt for precision. Result is <<8.
                uint16_t dist = isqrt((dc * dc + dr * dr) << 16);
                int32_t delta = abs((int32_t)dist - (int32_t)radius);
                if (delta < half_thickness) {
                    // This LED is inside the ring's thickness.
                    // 1.1: Calculate base intensity with a triangular profile.
                    // Using uint32_t for intensity to prevent overflow during calculations.
                    uint32_t intensity = (uint32_t)(UINT16_MAX) * (half_thickness - delta) / half_thickness;
                    // 1.2: Calculate a reduction factor that scales with age.
                    uint32_t reduction = ((uint32_t)age * INTENSITY_FADEOFF * 256) / WAVE_LIFESPAN;
                    // Prevent underflow
                    if (intensity > reduction) {
                        intensity -= reduction;
                    } else {
                        intensity = 0;
                    }
                    // 1.3: Add the final intensity to the buffer, saturating at max.
                    buffer_2d[r][c] = qadd16(buffer_2d[r][c], (uint16_t)intensity);
                }
            }
        }
    }

    // 2. Render
    for (uint8_t r = 0; r < NUM_ROWS; ++r) {
        for (uint8_t c = 0; c < NUM_COLUMNS; ++c) {
            uint16_t intensity = buffer_2d[r][c];
            uint16_t palette_index = remap(intensity, UINT16_MAX, PALETTE_LENGTH - 1);
            RGB color = palette[palette_index];
            set_led_2d(r, c, color.r, color.g, color.b);
        }
    }
}
