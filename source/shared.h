#pragma once

#include "rgb.h"
#include QMK_KEYBOARD_H
#include <quantum.h>
#include <keymap_spanish.h>
#include "keydefs.h"

// Handler function type definition
#define HANDLE_KEY_EVENT(name) void name(uint16_t keycode, bool down, keyrecord_t *record, bool*, bool*)
typedef HANDLE_KEY_EVENT((*KeyHandler));

enum OS {
	OSX = 0,
	WINDOWS
};

// Mods
enum modifier{
	M1_MOD,
	M2_MOD,
	M3_MOD,
	M4_MOD,
	M5_MOD,
	M6_MOD,
    M7_MOD,
	HYPER_MOD,
	CMD_MOD,
	ALT_MOD,
	CTR_MOD,
	S1_MOD,
    TILDE_MOD
};

enum layers {
	_BASE = 0,
	_M1,
	_M2,
	_M3,
    _NL,
	_M4,
	_M5,
	_M6,
    _M7,
	_HYP,
	_MOD,
	_CFG,
};

enum modMask {
	NO_M = 0,
	M1_M = 1 << 0,
	M2_M = 1 << 1,
	M3_M = 1 << 2,
	M4_M = 1 << 3,
	M5_M = 1 << 4,
	M6_M = 1 << 5,
	S1_M = 1 << 6
};

typedef struct Keypress {
	enum modifier mod;
	uint16_t mask;
	uint8_t layer;
	uint16_t skipTime;
	uint16_t keycode;
	uint16_t alternativeKeycode;
	uint16_t osKeycode;
	uint16_t alternativeOsKeycode;
	bool (*downHandler)( struct Keypress *);
	void (*upHandler)( struct Keypress *);
	bool down;
	bool interrupted;
	uint16_t time;
	struct Keypress *next;
} Keypress;


extern Keypress *activeMods;
extern enum OS os;
extern bool kb_synced;
extern bool kb_lock;
extern bool kb_suspended;
extern bool master;
extern bool caps_word;
extern uint8_t modsNumber;
extern uint8_t modMask;
extern uint8_t lastMask;
extern uint16_t passive;
extern bool app_switch;
extern bool shift;
extern bool caps_lock;
extern bool caps_state;
extern uint8_t win;  // 0 = false, 1 = waiting for second key 2 = active
extern bool hyper_lock;
extern uint16_t delete;  // there are different kinds of deletion (forward, backward, word, line...)
extern uint16_t registered_delete;
extern bool accent;
extern bool num_lock;
extern bool reactive_rgb;

void tap_key_sequence( uint16_t key );
void tap_os_dependent_key( uint16_t key );
void tap_alias_key( uint16_t keycode );
void tap_unshifted( uint16_t keycode );
void tap_generic_key(  uint16_t key );
void tap_sequence( uint16_t seqName );
void tap_unicode_key( uint16_t keyName );
void set_caps( bool enabled, bool lock, bool word );;
void register_generic_key(  uint16_t key );
void unregister_generic_key(  uint16_t key );
void register_os_dependent_key( uint16_t key );
void unregister_os_dependent_key( uint16_t key );
void tap_os_dependent_key( uint16_t key );
void app_switch_start( void );
void app_switch_end( void );
void del_sequence( uint16_t seqName );
uint16_t getOSKey( uint16_t keyName );
uint16_t getAliasKey( uint16_t keyName );
void flash_firmware( void );
void keypress_event( uint16_t keycode, bool down, keyrecord_t *record );
uint32_t decode_utf8_string(const char **str);
uint16_t basic_char_to_keycode(const char **str);
void send_string_sp(const char *str);
