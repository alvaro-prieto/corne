#include QMK_KEYBOARD_H
#include <keymap_spanish.h>
#include "keydefs.h"
#include "handlers.h"

//CODES:
//ALT A,  CMD G,  CONTROL C, SHIFT S, C+A LCA, ALGR (for Windows), LWIN, H (hyper)


//Definition of those shortcuts that perform the same action using different 
//key combinations accordint to the OS. Currently: {OSX, Windows}
static const uint16_t osKeys[][NUMBER_OF_OS] PROGMEM = { 
	//CMD_OS
	{ KC_LCMD, KC_LCTRL},
	//CTR_OS
	{ KC_LCTRL, KC_LWIN},
	//CUT
	{ G(ES_X), C(ES_X) },
	//COPY
	{ G(ES_C), C(ES_C) }, 
	//PASTE
	{ G(ES_V), C(ES_V) }, 
	//PLAIN_PASTE
	{ G(A(S(ES_V))), C(S(ES_V)) }, 
	//PASTE_IN_PLACE
	{ G(S(ES_V)), C(S(ES_V)) }, 
	//UNDO
	{ G(ES_Z), C(ES_Z) }, 
	//REDO
	{ G(S(ES_Z)), C(S(ES_Z)) },
	//SCRSHT1
	{ G(S(C(ES_4))), LWIN(S(ES_S)) },
	//SCRSHT2
	{ G(S(ES_5)), KC_PSCR },
	//EURO
	{ A(ES_E), ALGR(ES_E) },
	//REFRESH
	{ G(S(ES_R)), C(KC_F5) },
	//SAVE
	{ G(ES_S), C(ES_S) },
	//SELECT
	{ G(ES_A), C(ES_A) },
	//MUTE
	{ KC__MUTE, KC_MUTE },
	//VOL_UP
	{ KC__VOLUP, KC_VOLU },
	//VOL_DOWN
	{ KC__VOLDOWN, KC_VOLD },
	//CLS_APPS
	{ A(G(KC_ESC)), C(A(KC_DEL)) },
	//BR_UP
	{ KC_BRMU, KC_BRIU },
	//BR_DOWN
	{ KC_BRMD, KC_BRID },
	//W_LEFT
	{ A(KC_LEFT), A(KC_LEFT) },
	//W_RIGHT
	{ A(KC_RIGHT), A(KC_RIGHT) },
	//MONITOR
	{ KC_F19, C(S(KC_ESC))},
	//Z_IN
	{ G(ES_PLUS), C(ES_PLUS)},
	//Z_OUT
	{ G(ES_MINS), C(ES_MINS)},
	//TOP
	{ G(KC_UP), G(KC_HOME)}, //{ G(KC_HOME), G(KC_HOME)},
	//BOTTOM
	{ G(KC_DOWN), G(KC_END)}, //{ G(KC_END), G(KC_END)},
	//FIRST
	{ G(KC_LEFT), KC_HOME}, 
	//LAST
	{ G(KC_RIGHT), KC_END},
	//MINMZ
	{ G(ES_H), LWIN(KC_DOWN)},
	//CLS_W
	{ G(ES_W), A(KC_F4)},
	//SEARCH
	{ G(ES_F), C(ES_F)},
	//SRCH_NX
	{ G(ES_G), C(ES_G)},
	//SRCH_PR
	{ G(S(ES_G)), C(S(ES_G))},
	//RPLC 
	{ A(G(ES_F)), C(ES_H)},
	//SPOTL
	{ G(KC_SPC), LWIN(KC_S)},
	//SM_APP
	{ G(ES_GRV), KC_END},
	//EMOJI
	{ G(C(KC_SPC)), LWIN(ES_DOT)},
	//LOCK
	{ G(C(ES_Q)), LWIN(KC_L)},
	//WAVE
	{ A(ES_NTIL), ES_TILD},
	//DEL_W
	{ A(KC_BSPC), C(KC_BSPC)},
	//SNG_PR
	{ KC_F7, KC_MEDIA_PREV_TRACK},
	//SNG_NX
	{ KC_F9, KC_MEDIA_NEXT_TRACK},
	//PLAY
	{ KC_F8, KC_MEDIA_PLAY_PAUSE},
	//REBOOT
	{ C(KC_POWER), C(KC_POWER)},
	//POWER
	{ C(KC_POWER), KC_POWER},
	//WIN_L
	{ C(G(S(KC_L))), LWIN(KC_LEFT)},
	//WIN_R
	{ C(G(S(KC_R))), LWIN(KC_RIGHT)},
	//MAXMZ
	{ C(G(S(KC_M))), LWIN(KC_UP)},
	//RESTORE
	{ C(G(S(KC_U))), KC_NO},
	//CLS_APP
	{ G(ES_Q), A(KC_F4)},
	//WIN_CTR
	{ KC_LCTL, KC_LWIN},
	//DEL_LN
	{ G(KC_BSPC), C(S(ES_K))},
   
};

//Definition of those keys that should output more than one character, for example:  >=
static const uint16_t sequenceKeys[][SEQUENCE_MAX_LENGTH] PROGMEM = { 
	//MEMBER
	{ ES_MINS, ES_RABK, NULL_KEY }, 
	//FIELD
	{ ES_EQL, ES_RABK, NULL_KEY }, 
	//QUESTN
	{ ES_IQUE, ES_QUES, KC_LEFT },
	//EXCLAM
	{ ES_IEXL, ES_EXLM, KC_LEFT },
	//CMNT_OP
	{ ES_SLSH, ES_ASTR, NULL_KEY }, 
	//CMNT_CL
	{ ES_ASTR, ES_SLSH, NULL_KEY },  
	//ONEPLUS
	{ ES_PLUS, ES_1, NULL_KEY },  
	//ONEMINS   
	{ ES_MINS, ES_1, NULL_KEY },  
	//GR_EQ
	{ ES_RABK, ES_EQL, NULL_KEY },
	//LWR_EQ
	{ ES_LABK, ES_EQL, NULL_KEY },
	//NT_EQ
	{ ES_EXLM, ES_EQL, NULL_KEY },
	//GRT
	{ ES_RABK, NULL_KEY, NULL_KEY },
	//TAGCL
	{ ES_SLSH, ES_RABK, NULL_KEY },
	//LWR
	{ ES_LABK, NULL_KEY, NULL_KEY },
	//TAGOP
	{ ES_LABK, ES_SLSH, NULL_KEY },
	//SHF_L
	{ ES_LABK, ES_LABK, NULL_KEY },
	//SHF_R
	{ ES_RABK, ES_RABK, NULL_KEY },
	//NOT_EQ
	{ ES_EXLM, ES_EQL, NULL_KEY },
	//GRT_EQ
	{ ES_RABK, ES_EQL, NULL_KEY },
	//LSS_EQ
	{ ES_LABK, ES_EQL, NULL_KEY },
	
};

//ASCII characters are managed differently according to the OS. 
//In OSX usually they have a custom key combination, while in Windows 
//they can be managed using its ASCII index code.
static const uint16_t asciiKeys[][NUMBER_OF_OS] PROGMEM = { 
	//MIDLN
	{ A(ES_MINS), 150},
	//LONGLN
	{ A(S(ES_MINS)), 151},
	//TOPLN
	{ A(S(ES_J)), 175},
	//INFNT
	{ A(ES_5), 236},
	//ELLIPSIS
	{ A(S(ES_DOT)), 133},
	//PI
	{ A(ES_P), 227},
	//POUND
	{ A(S(ES_4)), 163},
	//DIFF
	{ A(ES_0), 8800},
	//SIM
	{ A(S(ES_0)), 247},
	//QUOT_R
	{ A(S(ES_CCED)), 175},
	//GREQ
	{ A(S(ES_LABK)), 242},
	//QUOT_L
	{ A(S(ES_ACUT)), 174},
	//LSEQ
	{ A(ES_LABK), 243},
	//DEGR
	{ A(S(KC_GRV)), 248},
	//REG
	{ A(ES_R), 174},
	//TM
	{ A(ES_H), 153},
	//BULLET
	{ A(S(ES_3)), 149},
	//CR
	{ A(ES_C), 169},
	
};


//returns the extendend shortcut keycode according to the current OS
static uint16_t getOSKey(uint16_t keyName ){
	return pgm_read_word( &( osKeys[ keyName - OS_INDEX -1 ][ os ] ) );
}; 

//Returns the uint16_t keycode or the ASCII index according to the current OS
static uint16_t getAsciiKey(uint16_t keyName ){
	return pgm_read_word( &( asciiKeys[ keyName - ASCII_INDEX -1 ][ os ] ));
};

//taps a sequence of keys. Useful for common key sequences such as <= -> and so on
static void tap_sequence(uint16_t seqName){
	char overflow = SEQUENCE_MAX_LENGTH;
	uint16_t currentKey;
	for(int i=0; i<SEQUENCE_MAX_LENGTH && overflow>0; i++, overflow--){
		currentKey = pgm_read_word( &( sequenceKeys[ seqName - SEQUENCE_INDEX -1 ][ i ] )); 
		if(currentKey == NULL_KEY) break;
		tap_code16( currentKey ); 
	}	
}; 

//taps a single ascii key according to the current OS
static void tap_ascii_key(uint16_t kc){
	uint16_t v = getAsciiKey( kc );
	if(os == OSX){
		tap_code16( v );
	}else{
		tap_ascii( v );
	}
}
