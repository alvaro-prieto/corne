#include QMK_KEYBOARD_H
#include <keymap_spanish.h>
#include "handlers.h"
#include "keydefs.h"
#include "dances.h"
#include "strings.h"

typedef struct{
	uint16_t keycode;
} tap_data;

typedef struct {
	enum tapType type;
	uint16_t kc1;	//1 tap keycode
	uint16_t kc2;	//2 taps keycode
} tapdance;


//Tap dance definitions. Order dependent (dances.h). A basic tap outpus the same keystrokes depending
//on the number of taps, a sequence tap outpus more than keystroke, a custom tap is handled manually
//and can be used, for example to output a different key combination according to the OS. New types of
//dances could be added if required.
static const tapdance tds[] PROGMEM ={
    //TDNULL
    {0,0,0},
	//AC_A
	{ ACCENT_TAP, ES_A, 0 },
	//AC_E
	{ ACCENT_TAP, ES_E, 0 },
	//AC_I
	{ ACCENT_TAP, ES_I, 0 },
	//AC_O
	{ ACCENT_TAP, ES_O, 0 },
	//AC_U
	{ ACCENT_TAP, ES_U, 0 },
	//RSET
	{ CUSTOM_TAP, NULLTAP, RESET_KB },
	//CMNT
	{ REPLACE_SEQ_TAP, CMNT_OP, CMNT_CL },
	//LN
	{ CUSTOM_TAP, LINE1, LINE2 },
	//TAGO
	{ REPLACE_SEQ_TAP, LWR, TAGOP },
	//TAGC
	{ REPLACE_SEQ_TAP, GRT, TAGCL },
	//DSK1
	{ BASIC_TAP, C(ES_1), C(S(ES_1)) },
	//DSK2
	{ BASIC_TAP, C(ES_2), C(S(ES_2)) },
	//DSK3
	{ BASIC_TAP, C(ES_3), C(S(ES_3)) },
	//ORD
	{ REPLACE_TAP, ES_MORD, ES_FORD },
	//SCR
	{ BASIC_OS_TAP, SCRSHT1, SCRSHT2 },
	//EXIT
	{ CUSTOM_TAP, CLOSE_WINDOW, CLOSE_APP },
	//GRTR
	{ REPLACE_SEQ_TAP, GRT_EQ, SHF_R },
	//LESS
	{ REPLACE_SEQ_TAP, LSS_EQ, SHF_L },
	//NTEQ
	{ CUSTOM_TAP, DIFF, SIM },
	//GRTP
	{ CUSTOM_TAP, QUOT_R, GREQ },
	//LSSP
	{ CUSTOM_TAP, QUOT_L, LSEQ },
	//STR12
	{ CUSTOM_TAP, STR1, STR2 },
	//APPS
	{ BASIC_OS_TAP, MONITOR, CLS_APPS },
	//CP
	{ BASIC_OS_TAP, COPY, CUT },
	//PST
	{ BASIC_OS_TAP, PST_CLEAN, PST_PLACE },
	//ADD
	{ REPLACE_TAP, ES_PLUS, ES_MINS },
	//MUL
	{ REPLACE_TAP, ES_ASTR, ES_SLSH },
	//CURL
	//{ REPLACE_TAP, ES_LCBR, ES_RCBR },
	//SQR
	//{ REPLACE_TAP, ES_LBRC, ES_RBRC },
	//PRN
	{ REPLACE_TAP, ES_LPRN, ES_RPRN },
	//MRK
	{ REPLACE_SEQ_TAP, QUESTN, EXCLAM },
	//ENYE
	{ REPLACE_LANG_TAP, ES_N, ES_NTIL },
	//DECI
	{ REPLACE_TAP, ES_DOT, ES_COMM },
	//G_DI
    { REPLACE_LANG_SEC_TAP, G, G_DIE },
    //DLT
    { BASIC_OS_TAP, TRASH, REMOVE },
};

//if the tap dance output should be a hold (instead of tap), the keycode
//is kept until the tap dance is over to be able to release it right then.
uint16_t holdTapCode = 0;


//Handler for custom tap dances
void tap_custom( int kc ){ // enum customTap kc ){  //not restricted to customTaps to allow keydef codes
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
		case LINE1:
			tap_code16( KC_F16);	//custom third party shortcut
			break;
		case LINE2:
			tap_code16( KC_F17 );	//custom third party shortcut
			break;
		case RESET_KB:
			reset_keyboard();
			break;
		case CLOSE_WINDOW:
			tap_code16( getOSKey( CLS_W ) );
			break;
		case CLOSE_APP:
			holdTapCode = getOSKey( CLS_APP );
			register_code16( holdTapCode );
			break;
		case STR1:
			send_string( STRING_1 );
			break;
		case STR2:
			send_string( STRING_2 );
			break;

		//────────────────── DEFAULT ──────────────────
		default:
			break;
	}
}



//Main tap dance event handler. According to the tap type and the tap count
//different actions are performed.
void dance_finished(qk_tap_dance_state_t *state, void *user_data) {
	//old code working without PROGMEM
	//tapdance td = tds[ ((tap_data*)user_data)->keycode ];
	//uint16_t kc = state->count == 1 ? td.kc1 : td.kc2;

	//Note that the tapdance struct has mixed size fields (byte, word, word),
	//so we can access in a safer maner.
	const tapdance * t = &tds[ ((tap_data*)user_data)->keycode ];
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


//Tap dance over callback
void dance_reset(qk_tap_dance_state_t *state, void *user_data) {
	if(holdTapCode){
		unregister_code16( holdTapCode );
	}
	holdTapCode = 0;
}

//Replace tap. Second tap replaces first output
void replace_each(qk_tap_dance_state_t *state, void *user_data) {
	const tapdance * t = &tds[ ((tap_data*)user_data)->keycode ];
    enum tapType type = (enum tapType) pgm_read_byte( &t->type );
    bool langTap = (type == REPLACE_LANG_TAP || type == REPLACE_LANG_SEC_TAP);
    bool tapEnabled = !(langTap && lang != ES);
    uint16_t kc[] = {pgm_read_word(&t->kc1),  pgm_read_word(&t->kc2)};
    uint16_t currentKc = kc[ tapEnabled ? state->count == 2 ? 1 : 0 : 0];

    switch (type){
        case REPLACE_TAP:
        case REPLACE_LANG_TAP:
            if(tapEnabled && state->count >= 2 && state->count <= 3){
                tap_code16( KC_BSPC );
            }
            if(state->count == 3) tap_code16( currentKc );
            tap_code16( currentKc );
            break;

        case REPLACE_SEQ_TAP:
        case REPLACE_LANG_SEC_TAP:
            if(tapEnabled && state->count >= 2 && state->count <= 3){
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


//Vowels can be accented if pressed twice
void accent_each(qk_tap_dance_state_t *state, void *user_data) {
	const tapdance * t = &tds[ ((tap_data*)user_data)->keycode ];
	uint16_t vowel = pgm_read_word( &t->kc1 );
	//uprintf("each %d\n", vowel);
	if( lang == ES){
		if( state->count == 1 ){
			tap_code16( vowel );
		}else if( state->count == 2 ){
			tap_code16( KC_BSPC );
			if(shift) unregister_code(KC_RSFT);
			tap_code16( ES_ACUT );
			if(shift) register_code(KC_RSFT);
			tap_code16( vowel );
			//this allows to prevent long tap dances
			//state->finished = true;
		}else if(state->count == 3 ){
            tap_code16( KC_BSPC );
            tap_code16( vowel );
        }
        if(state->count > 2){
            tap_code16( vowel );
        }
	}else{
		tap_code16( vowel );
	}
}

//Handler hooks. Add new tap dances here to be able to use them in the keymap.c file using TD( keycode )
//params: each, finish, reset, data
qk_tap_dance_action_t tap_dance_actions[] = {

    //Spanish accents
	[AC_A]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each,    NULL, NULL,  &((tap_data){ AC_A })),
	[AC_E]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each,    NULL, NULL,  &((tap_data){ AC_E })),
	[AC_I]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each,    NULL, NULL,  &((tap_data){ AC_I })),
	[AC_O]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each,    NULL, NULL,  &((tap_data){ AC_O })),
	[AC_U]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each,    NULL, NULL,  &((tap_data){ AC_U })),

    //Spanish special characters
	[ENYE]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each,   NULL, NULL,  &((tap_data){ ENYE })),
	[G_DI]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each,   NULL, NULL,  &((tap_data){ G_DI })),

    //replace dances (instant)
	[GRTR]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ GRTR })),
    [CMNT]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ CMNT })),
	[TAGO]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ TAGO })),
	[TAGC]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ TAGC })),
    [ORD]     = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ ORD  })),
	[ADD]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ ADD  })),
	[MUL]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ MUL  })),
    [LESS]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ LESS })),
	[PRN]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ PRN  })),
	[MRK]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ MRK  })),
    [DECI]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ DECI })),
//	[CURL]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ CURL })),
//	[SQR]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  replace_each, NULL, NULL,  &((tap_data){ SQR  })),

    //normal dances (processed when timeout is over)
	[RSET]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   RSET    })),
	[LN]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   LN      })),
	[DSK1]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DSK1    })),
	[DSK2]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DSK2    })),
	[DSK3]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DSK3    })),
	[SCR]     = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   SCR     })),
	[EXIT]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   EXIT    })),
	[NTEQ]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   NTEQ    })),
	[GRTP]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   GRTP 	})),
	[LSTP]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   LSTP    })),
	[STR12]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   STR12	})),
	[APPS]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   APPS 	})),
	[CP]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   CP 		})),
	[PST]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   PST 	})),
    [DLT]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DLT 	})),

};
