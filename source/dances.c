#include QMK_KEYBOARD_H
#include "shared.h"
#include "dances.h"
#include "strings.h"


//═══════════════════════════════════════════════════════════════
//   TAP-DANCE  (fast repeated key presses)
//═══════════════════════════════════════════════════════════════

typedef struct {
    uint16_t kc;
} tap;
typedef struct {
	enum tapType type;
	uint16_t kc1;	//1 tap keycode
	uint16_t kc2;	//2 taps keycode
} tapdance;

// if the tap dance output should be a hold (instead of tap), the keycode
// is kept until the tap dance is over to be able to release it right then.
uint16_t holdTapCode = 0;

// Tap dance definitions are order-dependent (defined in 'dances.h').
// A basic tap outputs the same keystrokes based on the number of taps,
// while a sequence tap outputs multiple keystrokes in a specific order.
// A custom tap is manually handled and allows for customized outputs,
// such as different key sequences based on the operating system.
// Additional types of dances can be added as needed.
static const tapdance tds[] PROGMEM ={
	// TDNULL ( skip action )
	{0,0,0},
	// RSET  ( double tap for QMK bootloader)
	{ CUSTOM_TAP, NULLTAP, RESET_KB },
	// CMNT  /*, */
	{ REPLACE_SEQ_TAP, CMNT_OP, CMNT_CL },
	// LN ---, ====
	{ BASIC_TAP, XENO(ES_C), XENO(ES_D) },
	// TAGO <, </
	{ REPLACE_SEQ_TAP, LWR, TAGOP },
	// TAGC >, />
	{ REPLACE_SEQ_TAP, GRT, TAGCL },
	// DSK1  swap to desk 1, move app to desk 1
	{ BASIC_TAP, C(ES_1), C(S(ES_1)) },
	// DSK2  swap to desk 2, move app to desk 2
	{ BASIC_TAP, C(ES_2), C(S(ES_2)) },
	// DSK3  swap to desk 3, move app to desk 3
	{ BASIC_TAP, C(ES_3), C(S(ES_3)) },
	// SCR   screenshot region, screenshot options
	{ BASIC_OS_TAP, SCRSHT1, SCRSHT2 },
	// EXIT  close window, close app
	{ CUSTOM_TAP, CLOSE_WINDOW, CLOSE_APP },
	// GRTR  >=, >>
	{ REPLACE_SEQ_TAP, GRT_EQ, SHF_R },
	// LESS  <= , <<
	{ REPLACE_SEQ_TAP, LSS_EQ, SHF_L },
	// STR12  string1, string2
	{ CUSTOM_TAP, STR1, STR2 },
    // STR34  string3 string4
	{ CUSTOM_TAP, STR3, STR4 },
	// CP  copy cut
	{ BASIC_OS_TAP, COPY, CUT },
	// PST  paste without format, plaste in place
	{ BASIC_OS_TAP, PST_CLEAN, PST_PLACE },
	// DLT  move to trash, permanently delete
	{ BASIC_OS_TAP, TRASH, REMOVE },
    // BOOT  turn off / reboot  using Xeno approach
    { BASIC_TAP, XENO(ES_I), XENO(ES_J) },
    // BOX  code box, using Xeno approach
    { BASIC_TAP, XENO(ES_G), XENO(ES_H) },

	//─────────── UNUSED ────────────
    // PRN  (,)
	// { REPLACE_TAP, ES_LPRN, ES_RPRN },
	// DECI  . ,
	// { REPLACE_TAP, ES_DOT, ES_COMM },
    // ORD  º,ª
	// { REPLACE_TAP, ES_MORD, ES_FORD },
	// NTEQ  ≠,≈
	// { CUSTOM_TAP, DIFF, SIM },
	// GRTP  », ≥
	// { CUSTOM_TAP, QUOT_R, GREQ },
	// LSTP  «, ≤
	// { CUSTOM_TAP, QUOT_L, LSEQ },
	// ADD  *,-
	// { REPLACE_TAP, ES_PLUS, ES_MINS },
	// MUL  *,/
	// { REPLACE_TAP, ES_ASTR, ES_SLSH },
	// CURL  {,}
	// { REPLACE_TAP, ES_LCBR, ES_RCBR },
	// SQR  [,]
	// { REPLACE_TAP, ES_LBRC, ES_RBRC },
	// MRK  ¿*?,¡*!
	// { SEQUENCE_TAP, QUESTN, EXCLAM },
};


// Handler for custom tap dances
void tap_custom( uint16_t kc ){  //not restricted to customTaps to allow keydef codes
    switch( kc ){
		//────────────────── UNICODE KEYS ──────────────────
		case SIM:
		case DIFF:
		case QUOT_R:
		case GREQ:
		case QUOT_L:
		case LSEQ:
			tap_unicode_key( kc );
			break;
		//────────────────── CUSTOM ACTIONS ──────────────────
		case RESET_KB:
            flash_firmware();
			break;
		case CLOSE_WINDOW:
			tap_code16( getOSKey( CLS_W ) );
			break;
		case CLOSE_APP:
			holdTapCode = getOSKey( CLS_APP );
			register_code16( holdTapCode );
			break;
		case STR1:
			send_string_sp( QSTR_1 );
			break;
		case STR2:
            send_string_sp( QSTR_2 );
			break;
		case STR3:
            send_string_sp( QSTR_3 );
			break;
		case STR4:
            send_string_sp( QSTR_4 );
			break;
		//────────────────── DEFAULT ──────────────────
		default:
			break;
	}
}


// Main tap dance event handler. According to the tap type and the tap count
// different actions are performed.
void dance_finished(tap_dance_state_t *state, void *user_data) {
	// Tapdance struct has mixed size fields (byte, word, word),
	const tapdance * t = &tds[ ((tap*)user_data)->kc ];
	enum tapType type = (enum tapType) pgm_read_byte( &t->type );
	uint16_t kc = pgm_read_word( state->count == 1 ? &t->kc1 : &t->kc2 );
	switch(type){
		case BASIC_TAP:
			tap_code16( kc );
		    break;
		case BASIC_OS_TAP:
			tap_code16( getOSKey( kc ) );
		    break;
		case SEQUENCE_TAP:
			tap_sequence( kc );
		    break;
		case CUSTOM_TAP:
			tap_custom( kc );
		    break;
		default:
		    break;
	}
}


// Tap dance over callback
void dance_reset(tap_dance_state_t *state, void *user_data) {
	if(holdTapCode){
		unregister_code16( holdTapCode );
	}
	holdTapCode = 0;
}

// Replace tap. Second tap replaces first output
void replace_each(tap_dance_state_t *state, void *user_data) {
	const tapdance * t = &tds[ ((tap*)user_data)->kc ];
	enum tapType type = (enum tapType) pgm_read_byte( &t->type );
	uint16_t kc[] = {pgm_read_word(&t->kc1),  pgm_read_word(&t->kc2)};
	uint16_t currentKc = kc[ state->count == 2 ? 1 : 0 ];
	switch (type){
		case REPLACE_TAP:
			if(state->count >= 2 && state->count <= 3){
				tap_code16( KC_BSPC );
			}
			if(state->count == 3) tap_code16( currentKc );
			tap_code16( currentKc );
			break;

		case REPLACE_SEQ_TAP:
			if(state->count >= 2 && state->count <= 3){
				del_sequence( kc[state->count-2] );
			}
			if(state->count == 3) tap_sequence( currentKc );
			tap_sequence( currentKc );
			break;

		default: break;
	}
	/* this allows to prevent long tap dances
	if(state->count == 2){
	   state->finished = true;
	} */
}


//══════════════════════════════════════════════
//                 HANDLER HOOKS
//══════════════════════════════════════════════

// Add new tap dances here to be able to use them in keymap.c
// using TD( keycode ).  Params: each, finish, reset, data

tap_dance_action_t tap_dance_actions[] = {

	//replace dances (instant)
	[GRTR]    = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  &((tap){ GRTR })),
	[CMNT]    = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  &((tap){ CMNT })),
	[TAGO]    = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  &((tap){ TAGO })),
	[TAGC]    = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  &((tap){ TAGC })),
	[LESS]    = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  &((tap){ LESS })),
//	[PRN]	  = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  PRN  ),
//	[DECI]	  = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  DECI ),
//	[ORD]     = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  ORD  ),
//	[ADD]	  = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  ADD  ),
//	[MUL]	  = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  MUL  ),
//	[CURL]	  = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  CURL ),
//	[SQR]	  = CUSTOM_TAP_FN(  replace_each, NULL, NULL,  SQR  ),

	//normal dances (processed when timeout is over)
	[RSET]    = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ RSET })),
	[LN]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ LN   })),
	[DSK1]    = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ DSK1 })),
	[DSK2]    = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ DSK2 })),
	[DSK3]    = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ DSK3 })),
	[SCR]     = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ SCR  })),
	[EXIT]    = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ EXIT })),
	[STR12]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ STR12 })),
    [STR34]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ STR34 })),
	[CP]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ CP   })),
	[PST]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ PST  })),
	[DLT]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ DLT  })),
    [BOOT]	  = CUSTOM_TAP_FN(  NULL, dance_finished, dance_reset,  &((tap){ BOOT })),


};
