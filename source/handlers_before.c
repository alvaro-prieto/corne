#include "shared.h"
#include "sync.h"
#include "handlers.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═══════════════════════════════════════════════════════════════
//   BEFORE KEY PROCESSING
//═══════════════════════════════════════════════════════════════

bool check_password(uint16_t keys[]);

// Function that gets called before processing any key event. It can perform several assertions,
// processing special cases, etc. Returns a boolean indicating wether the current key event should
// be processed or skipped (false = skip)
void before_handling(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){

    // New keypress means that old passive keys are invalidated
    passive_up();

    // Lazy mod. The windows key is not activated until a second key is pressed.
    if( !kb_lock && down && os == WINDOWS && win==1 ){
        register_code( KC_LWIN );
        win = 2;
    }

    // Keylog (unlock password && auto-diaeresis)
    static uint16_t keylog[ LOG_SIZE ] = {0};
    static uint8_t log_index = 0;

    if(down){
        keylog[log_index] = keycode;
        log_index = (log_index + 1) % LOG_SIZE;
        uint16_t keys[ LOG_SIZE ];
        for (uint8_t i = 0; i < LOG_SIZE; i++) {
            uint8_t idx = (log_index - 1 - i + LOG_SIZE) % LOG_SIZE;
            keys[i] = keylog[idx];
        }

        // diaeresis (averigüe, averigüé)
        if( !kb_lock
            && (keycode == ES_E || keycode == ES_I)
            && (
                (keys[3] == ES_G && keys[2] == TILDE && keys[1] == ES_U) ||
                (keys[4] == ES_G && keys[3] == TILDE && keys[2] == ES_U && keys[1] == TILDE))
            )
        {
            tap_code16( KC_BSPC );
            tap_code16( ES_DIAE );
            tap_code16( ES_U );
        }

        // unlock by typing your PASSWORD (config.h)
        if( kb_lock ){
            if( check_password( keys ) ){
                kb_lock = false;
                sync(SYNC_KB_LOCK, 0, 0);
                layer_move( _BASE );
                #ifdef RGB_MATRIX_ENABLE
                    rgb_set_notification( RGB_UNLOCK_NOTIFICATION );
                #endif
            }
            *handled = true;
            *stop_propagation = true;
        }
    }
}


//––––––––––––––––––––––––––––––––––––––––––––––––––
//   AUX FUNCTIONS
//––––––––––––––––––––––––––––––––––––––––––––––––––

// Converts the password string to keycodes
void password_to_keycodes(uint16_t *keycodes, uint8_t *length) {
    const char *ptr = PASSWORD;
    *length = 0;
    while (*ptr && *length < LOG_SIZE) {
        keycodes[*length] = basic_char_to_keycode(&ptr);
        if (keycodes[*length] == KC_NO) break; // Unsupported character (non-basic)
        (*length)++;
    }
}

// Checks if the last keys pressed match the password
bool check_password(uint16_t keys[]) {
    static uint16_t password_keycodes[LOG_SIZE];
    static uint8_t password_length = 0;
    static bool password_initialized = false;
    if (!password_initialized) {
        password_to_keycodes(password_keycodes, &password_length);
        password_initialized = true;
    }
    bool match = true;
    for (uint8_t i = 0; match && i < password_length; i++) {
        // keys[0] is the most recent key pressed
        if (keys[i] != password_keycodes[password_length - 1 - i]) {
            match = false;
            break;
        }
    }
    return match;
}
