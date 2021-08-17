#pragma once

#define CUSTOM_CODES_BASE_INDEX SAFE_RANGE
#define NUMBER_OF_OS 2
#define SEQUENCE_MAX_LENGTH 3 //Max number of characters that a sequence key can output 
//#define H(k) C(A(S(k)))  //H = Hyper

/*

What is the Hyper key for?
───────────────────────────

In my case it is going to be used as trigger to activate custom actions/shortcuts
depending on the focused application. For example, Hyper + I could mean "italic" in
Word while being "indent" in a code editor and "invert" in Photoshop. 

You can implement this kind of behaviour in programs such as Karabiner-Elements

*/

enum OS {
	OSX = 0,
	WINDOWS
};

extern enum OS os;  //it holds the OS mode of the keyboard. OSX by default


//Some keys could have a passive behaviour, for example, while being hold a key could
//output SHIFT keycode, but if any other key is pressed in conjuction shift is released
//to avoid acting as modifier. This enum is created only for readibility purposes
enum passive_keycodes{
	PASSIVE_SHIFT = KC_RSFT
};

//═══════════════════════════════════════════════════════════════
//   KEYCODE DECLARATIONS
//═══════════════════════════════════════════════════════════════
//most keycodes (except the ones manually handled) are order dependent. If you are 
//going to add new codes, add them at the bottom of each section and use them in the
//same order when handling them in handlers.c

enum custom_keycodes {

	//────────────── NULL KEY ──────────────
	//used when no action is required

	NULL_KEY = CUSTOM_CODES_BASE_INDEX, //SAFE_RANGE = 23849

	//────────────── MODIFIERS AND HOLD KEYS ──────────────
	MODIFIER_INDEX,
	M1,
	M2,
	M3,
	M4, 
	M5, 
	M6,  
	CMD,
	ALT,
	CTR,	
	S1,


	//────────────── SEQUENCE KEYS ──────────────
	SEQUENCE_INDEX,
	MEMBER,
	FIELD,
	QUESTN,
	EXCLAM,
	CMNT_OP,
	CMNT_CL,
	ONEPLUS,
	ONEMINS,
	GR_EQ,
	LWR_EQ,
	NT_EQ,
	GRT,
	TAGCL,
	LWR,
	TAGOP,
	SHF_L,
	SHF_R,
	NOT_EQ,
	GRT_EQ,
	LSS_EQ,


	//────────────── OS DEPENDENT KEYS ──────────────
	OS_INDEX,
	CMD_OS,
	CTR_OS,
	CUT,
	COPY,
	PASTE,
	PST_CLEAN,
	PST_PLACE,
	UNDO,
	REDO,
	SCRSHT1,
	SCRSHT2,
	EURO,
	REFRESH,
	SAVE,
	SELECT,
	MUTE,
	VOL_UP,
	VOL_DOWN,
	CLS_APPS,
	BR_UP,
	BR_DOWN,
	W_LEFT,
	W_RIGHT,
	MONITOR,
	Z_IN,
	Z_OUT,
	TOP,
	BOTTOM,
	FIRST,
	LAST,
	MINMZ,
	CLS_W,
	SEARCH,
	SRCH_NX,
	SRCH_PR,
	RPLC,
	SPOTL,
	SM_APP,
	EMOJI,
	LOCK,
	WAVE,
	DEL_W,
	SNG_PR,
	SNG_NX,
	PLAY,
	REBOOT,
	POWER,
	WIN_L,
	WIN_R,
	MAXMZ,
	RESTORE,
	CLS_APP,
	WIN_CTR,
	DEL_LN,


	//────────────── ASCII KEYS ──────────────
	ASCII_INDEX,
	MIDLN,
	LONGLN,
	TOPLN,
	INFNT,
	ELLIPSIS,
	PI,
	POUND, 
	DIFF,
	SIM,
	QUOT_R,
	GREQ,
	QUOT_L,
	LSEQ,
	DEGR,
	REG,
	TM,
	BULLET,
	CR,


	//────────────── CUSTOM HANDLED KEYS ──────────────
	APPLE_F1,
	APPLE_F2,
	APPLE_F3,
	APPLE_F4,
	APPLE_F5,
	APPLE_F6,
	APPLE_F7,
	APPLE_F8,
	APPLE_F9,
	APPLE_F10,
	APPLE_F11,
	APPLE_F12,
	SWAP_OS,
	SCR_OFF,
	NEXT_APP,
	PREV_APP,
	DESK1,
	DESK2,
	DESK3,
	SHW_DSK,
	EXPS,
	BOOT,
	H_LOCK,
	BORDER,
	KBLOCK,
	RGB_NEXT,
	RGB_PREV,
	DEL,
	DOT,

};


static void tap_sequence(uint16_t seqName );
static uint16_t getOSKey(uint16_t keyName );
static void tap_sequence(uint16_t seqName);
static void tap_ascii_key(uint16_t keyName);






