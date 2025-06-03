#include "shared.h"


//═══════════════════════════════════════════════════════════════
//	KEYCODE DEFINITIONS
//═══════════════════════════════════════════════════════════════


//   OS-DEPENDENT KEYBOARD SHORTCUTS
//––––––––––––––––––––––––––––––––––––––––––––––––––
// Reminder: ALT A,  CMD G,  CONTROL C, SHIFT S
// Currently: {OSX, Windows}

const uint16_t osKeys[][NUMBER_OF_OS] PROGMEM = {
	//CMD_OS
    { KC_LCMD, KC_LCTL},
	//CTR_OS
	{ KC_LCTL, KC_LWIN},
	//CUT
	{ G(ES_X), C(ES_X) },
	//COPY
	{ G(ES_C), C(ES_C) },
	//PASTE
	{ G(ES_V), C(ES_V) },
	//PST_CLEAN
	{ G(A(S(ES_V))), C(S(ES_V)) },
	//PST_PLACE
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
	{ KC_KB_MUTE, KC_MUTE },
	//VOL_UP
	{ KC_KB_VOLUME_UP, KC_VOLU },
	//VOL_DOWN
	{ KC_KB_VOLUME_DOWN, KC_VOLD },
	//CLS_APPS
	{ A(G(KC_ESC)), C(A(KC_DEL)) },
	//W_LEFT
	{ A(KC_LEFT), A(KC_LEFT) },
	//W_RIGHT
	{ A(KC_RIGHT), A(KC_RIGHT) },
	//MONITOR
	{ XENO(KC_E), C(S(KC_ESC))},
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
	//TRASH
	{ G(KC_BSPC), KC_DEL},
	//REMOVE
	{ A(G(KC_BSPC)), S(KC_DEL)},
    //REBOOT
	//{ C(KC_POWER), C(KC_POWER)},
	//POWER
	//{ C(KC_POWER), KC_POWER},
};


//  KEY SEQUENCES
//––––––––––––––––––––––––––––––––––––––––––––––––––
// Keys producing multi-character output (e.g., ->)

const uint16_t sequenceKeys[][SEQUENCE_MAX_LENGTH] PROGMEM = {
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
	//BCKQT
	{ ES_GRV, KC_SPC, NULL_KEY },
	//POW
	{ ES_CIRC, KC_SPC, NULL_KEY },

};



//   UNICODE
//––––––––––––––––––––––––––––––––––––––––––––––––––

// UNICODE characters are managed differently according to the OS. In OSX you can set
// Unicode Hex Input in the keyboard device settings, but for most used characters you
// can use their custom key combination if you don't want to mess with strange input modes.
// In Windows unicode characters can be managed using its code https://unicode-table.com
// and installing WinCompose

const uint16_t unicodeKeys[][NUMBER_OF_OS] PROGMEM = {
	//MIDLN
	{ A(ES_MINS), 0x2013},
	//LONGLN
	{ A(S(ES_MINS)), 0x2014},
	//TOPLN
	{ A(S(ES_J)), 0x00AF},
	//INFNT
	{ A(ES_5), 0x221E},
	//ELLIPSIS
	{ A(S(ES_DOT)), 0x2026},
	//PI
	{ A(ES_P), 0x03C0},
	//POUND
	{ A(S(ES_4)), 0x00A3},
	//DIFF
	{ A(ES_0), 0x2260},
	//SIM
	{ A(S(ES_0)), 0x2248},
	//QUOT_R
	{ A(S(ES_CCED)), 0x00BB},
	//GREQ
	{ A(S(ES_LABK)), 0x2265},
	//QUOT_L
	{ A(S(ES_ACUT)), 0x00AB},
	//LSEQ
	{ A(ES_LABK), 0x2264},
	//DEGR
	{ A(S(KC_GRV)), 0x00B0},
	//REG
	{ A(ES_R), 0x00AE},
	//TM
	{ A(ES_H), 0x2122},
	//BULLET
	{ A(S(ES_3)), 0x2022},
	//CR
	{ A(ES_C), 0x00A9},
};

//   ALIAS
//––––––––––––––––––––––––––––––––––––––––––––––––––

// Alias type keys are keys or combinations of keys that have been given
// an identifying name for legibility purposes. Some of them are xeno keys,
// those won't work unless you configure them in an external tool
// ⚠ NOTE: xeno keys declaration in keydefs.h
const uint16_t aliasKeys[] PROGMEM = {
    //UNDERSC
    S(ES_MINS),
    //TGL_APP   Toggle last app
    XENO(KC_A),
    //UNTAB     shift(tab)
    S(KC_TAB),
    //FLASH     Quick close private content
    XENO(KC_F),
    //UNICODE
    XENO(KC_L),
};
