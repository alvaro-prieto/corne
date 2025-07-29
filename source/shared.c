#include "shared.h"
#include "keydefs.h"
#include "sync.h"
#include "handlers.h"
#include "deferred_exec.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═══════════════════════════════════════════════════════════════
//	SHARED  (variables and functions shared between files)
//═══════════════════════════════════════════════════════════════

Keypress *activeMods = NULL;
uint8_t modMask = 0;
uint8_t lastMask = 0;
enum OS os = OSX;
bool kb_synced = false;
bool kb_lock = LOCK_KB_ON_BOOT;
bool kb_suspended = false;
bool master = false;
bool caps_word = false;
uint16_t passive = 0;
bool app_switch = false;
bool shift = false;
bool caps_lock = false;
bool caps_state = false;
uint8_t win = 0;  // 0 = false, 1 = waiting for second key 2 = active
bool hyper_lock = false;
uint16_t delete = 0;  // there are different kinds of deletion (forward, backward, word, line...)
uint16_t registered_delete = 0;
bool accent = false;
bool num_lock = false;
bool reactive_rgb = false;
deferred_token suspend_timer;

//–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

// Starts the CMD+TAB / ALT+TAB app switcher
void app_switch_start( void ){
  if(!app_switch){
	app_switch = true;
	register_code16( os == OSX ? KC_LCMD: KC_LOPT );
  }
}

// Ends the CMD+TAB / ALT+TAB app switcher
void app_switch_end( void ){
  if(app_switch){
	app_switch = false;
	unregister_code16( os == OSX ? KC_LCMD: KC_LOPT );
  }
}

// Internal function that toggles caps_state using only KC_CAPS instead of
// shift to prevent inteferences with other symbols, tildes, etc.
static void set_caps_state(bool enabled, bool lock, bool word){
    if(caps_state != enabled) {
        tap_code(KC_CAPS);
    }
	caps_state = enabled;
	#ifdef RGB_MATRIX_ENABLE
        if(lock){
            rgb_set_indicator(RGB_CAPS_WORD, false);
	        rgb_set_indicator(RGB_CAPS_LOCK, enabled);
        }
        if(word){
            rgb_set_indicator(RGB_CAPS_LOCK, false);
            rgb_set_indicator(RGB_CAPS_WORD, enabled);
            if(enabled){
                rgb_set_notification( RGB_CAPS_WORD_NOTIFICATION );
            }
        }
	#endif
}

// Centralization of caps management (shift, caps lock and caps word)
void set_caps(bool enabled, bool lock, bool word){
    if(lock || word){
        set_caps(false, false, false); //prevent hold shift
        set_caps_state(enabled, lock, word);
        caps_lock = lock ? enabled : 0;
        caps_word = word ? enabled : 0;
    }else{
        // hold shift (S1) has to be very fast, so it can't rely on
        // caps lock (since it has a delay on macOs)
        if(caps_lock || caps_word) return;
        enabled ? register_code16(KC_RSFT) : unregister_code16(KC_RSFT);
        shift = enabled;
    }
}

// Hold down an OS dependent key
void register_os_dependent_key( uint16_t key ){
  uint16_t code = getOSKey( key );
  if( code ) register_code16( code );
}

// Releases an OS dependent key
void unregister_os_dependent_key( uint16_t key ){
  uint16_t code = getOSKey( key );
  if( code ) unregister_code16( code );
}

// Tap an OS dependent key
void tap_os_dependent_key( uint16_t key ){
  uint16_t code = getOSKey( key );
  if( code ) tap_code16( code );
}

// Tap an alias key
void tap_alias_key( uint16_t keycode ){
    tap_code16( getAliasKey(keycode) );
}

// Smart tap a key according to its type. (This function can be improved to work with more types)
// Currently working for: normal keys + os keys + alias keys
void tap_generic_key(  uint16_t key ){
    if(key > FIRST_OS_INDEX && key < LAST_OS_INDEX){
        tap_os_dependent_key( key );
    }else if(key > FIRST_ALIAS_INDEX && key < LAST_ALIAS_INDEX){
        tap_code16( getAliasKey(key) );
    }else{
        tap_code16( key );
    }
}

// Tap a key preventing shift hold state
void tap_unshifted( uint16_t keycode ){
    if(shift) unregister_code16( KC_RSFT );
    tap_code16( keycode );
    if(shift) register_code16( KC_RSFT );
}

// Smart register a key according to its type. (This function can be improved to work with more types)
// Currently working for: normal keys + os keys
void register_generic_key(  uint16_t key ){
    if(key > FIRST_OS_INDEX && key < LAST_OS_INDEX){
        register_os_dependent_key( key );
    }else{
        register_code16( key );
    }
}

// Smart unregister a key according to its type. (This function can be improved to work with more types)
// Currently working for: normal keys + os keys
void unregister_generic_key(  uint16_t key ){
    if(key > FIRST_OS_INDEX && key < LAST_OS_INDEX){
        unregister_os_dependent_key( key );
    }else{
        unregister_code16( key );
    }
}

// Enables the mod layer
static inline void enable_mod_layer( void ){
  layer_on(_MOD);
}

// Disables the mod layer
static inline void disable_mod_layer( void ){
  layer_off(_MOD);
}

/*
//Tap a shifted code (not needed in the current implementation)
void tap_shifted_code16( uint16_t key ){
    uint8_t mod_state = get_mods();
    del_mods(MOD_MASK_SHIFT);
    register_code16( KC_RSFT );
    tap_code16( key );
    unregister_code16( KC_RSFT );
    set_mods(mod_state);
}
*/

// Returns the extendend shortcut keycode according to the current OS
uint16_t getOSKey(uint16_t keyName ){
	return pgm_read_word( &( osKeys[ keyName - FIRST_OS_INDEX -1 ][ os ] ) );
};

// Returns the uint16_t keycode or the unicode number according to the current OS
static uint16_t getUnicodeKey(uint16_t keyName ){
	return pgm_read_word( &( unicodeKeys[ keyName - FIRST_UNICODE_INDEX -1 ][ os ] ));
};

// Returns the uint16_t keycode for an alias key
uint16_t getAliasKey(uint16_t keyName ){
	return pgm_read_word( &( aliasKeys[ keyName - FIRST_ALIAS_INDEX -1 ] ));
};

// Shared code among tap_sequence and del_sequence, this function is not public
static void tap_sequence_internal(uint16_t seqName, bool delete){
	uint8_t overflow = SEQUENCE_MAX_LENGTH;
	uint16_t currentKey;
	for(uint8_t i=0; i<SEQUENCE_MAX_LENGTH && overflow>0; i++, overflow--){
		currentKey = pgm_read_word( &( sequenceKeys[ seqName - FIRST_SEQUENCE_INDEX -1 ][ i ] ));
		if(currentKey == NULL_KEY) break;
		tap_code16( delete ? KC_BSPC : currentKey );
	}
}

// Taps a sequence of keys. Useful for common key sequences such as <= -> and so on
void tap_sequence(uint16_t seqName){
	tap_sequence_internal(seqName, false);
};

// Deletes the same amount of characters of a sequence of keys
void del_sequence(uint16_t seqName){
	tap_sequence_internal(seqName, true);
};


// Function that outputs a character using its Unicode number in Windows or its key
// combination in macOS. It could behave in the same way in different OS (using only its unicode),
// but it requires to set the keyboard settings in Unicode Hex Input mode in OSX, which I dislike.
void tap_unicode_key(uint16_t kc){
	uint16_t u = getUnicodeKey( kc );
	if(os == OSX){
		tap_code16( u );
	}else{
		register_unicode( u );
	}
}

// Set the keyboard in bootloader mode
uint32_t tigger_bootloader(uint32_t trigger_time, void *cb_arg) {
    reset_keyboard();
    return 0;
}

// Trigger the bootloader mode, display its flashing rgb map and fire the Xeno signal to
// automatically copy the .uf2 file into the external drive according to the firmware model
void flash_firmware(void){
    #ifdef RGB_MATRIX_ENABLE
	    rgb_set_theme_no_sync(RGB_FLASHING);
    #endif
    // Corne rgb rp2040 = F23
    // Unicorne boardsource = F24
    if(CORNE_FIRMWARE_MODEL){
        tap_code16( XENO(KC_F22 + CORNE_FIRMWARE_MODEL) );
    }
	defer_exec( 150, tigger_bootloader, NULL );
}

// Executed in the master when the keyboard changes its suspend state (inactivity)
void keyboard_suspended( bool suspended ){
    kb_suspended = suspended;
    sync( SYNC_KB_SUSPEND, suspended, 0 );
    #ifdef RGB_MATRIX_ENABLE
        rgb_set_suspend( suspended );
    #endif
}

// Executed when the keyboard should be suspended due to inactivity
uint32_t suspend_timer_over( uint32_t trigger_time, void *cb_arg ) {
    keyboard_suspended(true);
    return 0;
}

// Executed on the master when a key event is detected to prevent suspension, reactive animations, etc.
void keypress_event( uint16_t keycode, bool down, keyrecord_t *record ){
    if(kb_lock || !master )  return;
    if(suspend_timer) cancel_deferred_exec( suspend_timer );
    suspend_timer = defer_exec( SLEEP_TIMEOUT, suspend_timer_over, NULL );
    bool prev_kb_suspended = kb_suspended;
    if(prev_kb_suspended){
        keyboard_suspended(false);
    }
    prev_kb_suspended = false;

    #ifdef RGB_MATRIX_ENABLE
    // Reactivity is the slave's ability to detect key presses on the master.
    // Currently, is used only for RGB, but it could serve other purposes.
    if( reactive_rgb && !IS_COMBOEVENT(record->event) ){
        const uint8_t row = record->event.key.row;
        const uint8_t col = record->event.key.col;
        rgb_keyboard_event( col, row, down);
        sync( SYNC_KB_KEYPRESS, (row << 3) | (col & 0x07), down );
    }
    #endif
}

// Decodes a UTF-8 encoded character into its Unicode value
uint32_t decode_utf8_string(const char **str) {
    uint32_t code = 0;
    uint8_t c = **str;
    if ((c & 0x80) == 0) {
        code = c;
        (*str)++;
    } else if ((c & 0xE0) == 0xC0) {  // 2-byte
        code = ((c & 0x1F) << 6) | ((*str)[1] & 0x3F);
        (*str) += 2;
    } else if ((c & 0xF0) == 0xE0) {  // 3-byte
        code = ((c & 0x0F) << 12) | (((*str)[1] & 0x3F) << 6) | ((*str)[2] & 0x3F);
        (*str) += 3;
    }
    return code;
}

// Converts a UTF-8 character to its corresponding keycode (basic latin + ñ)
uint16_t basic_char_to_keycode(const char **str) {
    uint32_t unicode = decode_utf8_string(str);
    if (unicode >= 'a' && unicode <= 'z') {
        return KC_A + (unicode - 'a');
    } else if (unicode == 0x00F1) {
        return ES_NTIL;
    }
    return KC_NO;
}

#include "send_string_spanish.c"
// Similar to send_string, but for Spanish input mode
void send_string_sp(const char *str){
    send_string_spanish(str);
}
