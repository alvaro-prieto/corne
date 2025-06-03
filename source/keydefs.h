#pragma once

#define CUSTOM_CODES_BASE_INDEX SAFE_RANGE
#define NUMBER_OF_OS 2
#define SEQUENCE_MAX_LENGTH 3 // Maximum number of characters a sequence key can output
#define HYPER(k) C(A(S(k)))
#define XENO(k) G(C(A(S(k))))

//═══════════════════════════════════════════════════════════════
//   CUSTOM KEYCODES
//═══════════════════════════════════════════════════════════════

// Most keycodes, except those handled manually, are order-dependent.
// When adding new keycodes, append them to the bottom of each section
// and maintain the same order when handling them in `handlers.c`.

enum custom_keycodes {

	//────────────── NULL KEY ──────────────
	// used when no action is required
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
    TEST,
    TEST2,
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
	RGBT_NEXT,
	RGBT_PREV,
  	RGBA_NEXT,
	RGBA_PREV,
    RGBR_NEXT,
	RGBR_PREV,
	DEL,
    RGB_UP,
    RGB_DOWN,
    NLOCK,
    TERMINAL,

    /*────────────── XENO KEYS ──────────────
    [] Means that alias name is not explicit (could be in a TD, etc)

    TGL_APP     XN(KC_A)     Toggle last app
    💀 DBLN        XN(KC_B)     ═ Long equal
    [LINE1]     XN(KC_C)     ───────────────
    [LINE2]     XN(KC_D)     ═══════════════
    MONITOR     XN(KC_E)     System monitor
    FLASH       XN(KC_F)     Quick close private windows
    [BOX1]      XN(KC_G)     Single line box
    [BOX2]      XN(KC_H)     Double line box
    [OFF]       XN(KC_I)     Turn off the computer
    [REBOOT]    XN(KC_J)     Reboot the computer
    💀 CROSS       XN(KC_K)     ┼
    UNICODE     XN(KC_L)     Sample of useful unicode characters
    TERMINAL    XN(KC_M)     Open terminal
    💀 PST_CLEAN   XN(KC_N)     Pegar sin formato

    */
};

/*
Hyper Modifier:
─────────────────────────────────
The Hyper modifier serves as a universal trigger for custom actions and shortcuts,
adapting to the focused application. For example, Hyper + I might apply italics in Word,
indent text in a code editor, or invert colors in Photoshop. This functionality can be
achieved using tools like Karabiner-Elements, Keyboard Maestro, Hammerspoon, and BetterTouchTool.

Xeno Modifier:
────────────────────────────────
The Xeno modifier (from Greek "external, foreign") is designed to trigger system-level
or application-specific functions that lack built-in shortcuts. It works in conjunction
with external automation tools to enable these actions.
*/


extern const uint16_t osKeys[][NUMBER_OF_OS] PROGMEM;
extern const uint16_t sequenceKeys[][SEQUENCE_MAX_LENGTH] PROGMEM;
extern const uint16_t aliasKeys[] PROGMEM;
extern const uint16_t unicodeKeys[][NUMBER_OF_OS] PROGMEM;







