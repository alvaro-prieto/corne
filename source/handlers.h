#pragma once

void tap_key_sequence( uint16_t key );
bool mod_key_handler( uint16_t keycode, bool down, keyrecord_t *record );
bool hold_key_handler( uint16_t keycode, bool down, keyrecord_t *record );
void interruptMods( void );
void hyper( bool down );
void toggle_hyper_lock( void );
void passive_down( uint16_t key );
void passive_up( void );

enum layers{
	_BASE = 0,
	_M1,
	_M2,
	_M3,
	_M4,
	_M5,
	_M6,
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
	HYPER_MOD,
	CMD_MOD,
	ALT_MOD,
	CTR_MOD,
	S1_MOD
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

enum languages {
	ES = 0,
	EN
};

//active holds and mods are kept in queue, so if any other key is pressed 
//in combination their alternative behavior could be prevented
Keypress *activeMods	= NULL;
unsigned char modsNumber = 0;
unsigned short modMask = 0;
unsigned short lastMask = 0;
uint16_t passive = 0;
bool app_switch = false;
bool shift = false;
bool caps_lock = false;
bool cmd = false;
unsigned char win = 0;  //0 = false, 1 = waiting for second key 2 = active  
int RGB_current_mode;  //no se si se usa... TO-DO
bool hyper_lock = false;
bool kb_lock = false;
unsigned char lang = ES;






