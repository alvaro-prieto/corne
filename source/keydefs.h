#pragma once

#define CUSTOM_CODES_BASE_INDEX SAFE_RANGE
#define NUMBER_OF_OS 2
#define SEQUENCE_MAX_LENGTH 3 //Max number of characters that a sequence key can output
#define H(k) C(A(S(k)))     //H = Hyper
#define X(k) G(C(A(S(k))))  //X = Xeno

/*

What is the Hyper modifier for?
───────────────────────────
In my case it is going to be used as trigger to activate custom actions/shortcuts
depending on the focused application. For example, Hyper + I could mean "italic" in
Word while being "indent" in a code editor and "invert" in Photoshop.

You can implement this kind of behaviour in programs such as Karabiner-Elements,
Keyboard Maestro, Hammerspoon, Better Touch Tool, ...


What is the Xeno modifier for?
───────────────────────────
There are many functions at the level of the operating system or applications that
do not have a direct shortcut. To make up for this, the Xeno modifier (Greek "external, foreign")
has been created, which allows these behaviors to be created with the help of external programs.

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
	FIRST_MODIFIER_INDEX,
	M1,
	M2,
	M3,
	M4,
	M5,
	M6,
    M7,
	CMD,
	ALT,
	CTR,
	S1,
    TILDE,
	LAST_MODIFIER_INDEX,

	//────────────── SEQUENCE KEYS ──────────────
	FIRST_SEQUENCE_INDEX,
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
	BCKQT,
	POW,
	//G,
	//G_DIE,
	LAST_SEQUENCE_INDEX,

	//────────────── OS DEPENDENT KEYS ──────────────
	FIRST_OS_INDEX,
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
	//REBOOT,
	//POWER,
	WIN_L,
	WIN_R,
	MAXMZ,
	RESTORE,
	CLS_APP,
	WIN_CTR,
	DEL_LN,
	TRASH,
	REMOVE,
	LAST_OS_INDEX,

	//────────────── UNICODE KEYS ──────────────
	FIRST_UNICODE_INDEX,
    DBLN,
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
    CROSS,
    DCROSS,
    QCROSS,
	LAST_UNICODE_INDEX,

	//────────────── ALIAS KEYS ──────────────
    FIRST_ALIAS_INDEX,
    UNDERSC,
    TGL_APP,
    UNTAB,
    FLASH,
    UNICODE,
    LAST_ALIAS_INDEX,

	//────────────── CUSTOM HANDLED KEYS ──────────────
	FIRST_CUSTOM_INDEX,
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
	H_LOCK,
	KBLOCK,
	OSLOCK,
	RGB_NEXT,
	RGB_PREV,
	DEL,
	DOT,
    RGB_UP,
    RGB_DOWN,
    NLOCK,

    /*────────────── XENO KEYS ──────────────
    [] Means that alias name is not explicit (could be in a TD, etc)

    TGL_APP     X(KC_A)     Toggle last app
    DBLN        X(KC_B)     ═ Long equal
    [LINE1]     X(KC_C)     ───────────────
    [LINE2]     X(KC_D)     ═══════════════
    MONITOR     X(KC_E)     System monitor
    FLASH       X(KC_F)     Quick close private windows
    [BOX1]      X(KC_G)     Single line box
    [BOX2]      X(KC_H)     Double line box
    [OFF]       X(KC_I)     Turn off the computer
    [REBOOT]    X(KC_J)     Reboot the computer
    CROSS       X(KC_K)     ┼
    UNICODE     X(KC_L)     Sample of useful unicode characters

    */
};


static void tap_sequence(uint16_t seqName );
static void del_sequence(uint16_t seqName );
static uint16_t getOSKey(uint16_t keyName );
static void tap_sequence(uint16_t seqName);
static void tap_unicode_key(uint16_t keyName);






