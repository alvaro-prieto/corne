#pragma once

enum layers{
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

typedef struct Keypress{
	enum modifier mod;
	short mask;
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


enum modMask{
	NO_M = 0,
	M1_M = 1 << 0,
	M2_M = 1 << 1,
	M3_M = 1 << 2,
	M4_M = 1 << 3,
	M5_M = 1 << 4,
	M6_M = 1 << 5,
	S1_M = 1 << 6
};


//signatures
void tap_key_sequence( uint16_t key );
bool mod_is_active(Keypress *kp);
bool mod_key_handler( uint16_t keycode, bool down, keyrecord_t *record );
bool hold_key_handler( uint16_t keycode, bool down, keyrecord_t *record );
void interrupt_mods( void );
void hyper( bool down );
void toggle_hyper_lock( void );
void passive_down( uint16_t key );
void passive_up( void );
void set_caps(bool enabled, bool lock, bool word);



//active holds and mods are kept in queue, so if any other key is pressed
//in combination their alternative behavior could be prevented
static Keypress *activeMods	= NULL;
static unsigned char modsNumber = 0;
static unsigned short modMask = 0;
static unsigned short lastMask = 0;
static uint16_t passive = 0;
static bool app_switch = false;
static bool shift = false;
static bool caps_lock = false;
static bool caps_word = false;
static bool caps_state = false;
static unsigned char win = 0;  //0 = false, 1 = waiting for second key 2 = active
static bool hyper_lock = false;
static bool kb_lock = false;
static uint16_t delete = 0;  //there are different kinds of deletion (forward, backward, word, line...)
static uint16_t registered_delete = 0;
static bool accent = false;
static bool num_lock = false;









