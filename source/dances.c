#include QMK_KEYBOARD_H
#include <keymap_spanish.h>
#include "handlers.h"
#include "keydefs.h"
#include "dances.h"

typedef struct{
	uint16_t keycode;
} tap_data;

typedef struct {
	enum tapType type;
	uint16_t kc1;	//1 tap keycode
	uint16_t kc2;	//2 taps keycode
} tapdance;


//Tap dance definitions. Order dependent (keydances.h). A basic tap outpus the same keystrokes depending 
//on the number of taps, a sequence tap outpus more than keystroke, a custom tap is handled manually 
//and can be used, for example to output a different key combination according to the OS. New types of
//dances could be added if required.
tapdance tds[] ={
	//ARROWS
	{ SEQUENCE_TAP, MEMBER, FIELD },
	//COMNT
	{ SEQUENCE_TAP, CMNT_OP, CMNT_CL },
	//LINES
	{ CUSTOM_TAP, LINE1, LINE2 },
	//RSET
	{ CUSTOM_TAP, NULLTAP, RESET_KB },
	//TAG_O
	{ SEQUENCE_TAP, LWR, TAGOP },
	//TAG_C
	{ SEQUENCE_TAP, GRT, TAGCL },
	//DSK1
	{ BASIC_TAP, C(ES_1), C(S(ES_1)) },
	//DSK2
	{ BASIC_TAP, C(ES_2), C(S(ES_2)) },
	//DSK3
	{ BASIC_TAP, C(ES_3), C(S(ES_3)) },
	//ORD
	{ BASIC_TAP, ES_MORD, ES_FORD },  
	//SCR
	{ BASIC_OS_TAP, SCRSHT1, SCRSHT2 },
	//EXIT
	{ CUSTOM_TAP, CLOSE_WINDOW, CLOSE_APP },
	//GRTR
	{ SEQUENCE_TAP, GRT_EQ, SHF_R },
	//LESS 
	{ SEQUENCE_TAP, LSS_EQ, SHF_L },	
	//NTEQ
	{ CUSTOM_TAP, DIFF, SIM },
	//GRTP
	{ CUSTOM_TAP, QUOT_R, GREQ },
	//LSSP
	{ CUSTOM_TAP, QUOT_L, LSEQ },
	//STR12
	{ CUSTOM_TAP, STR1, STR2 },
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

};

//if the tap dance output should be a hold (instead of tap), the keycode
//is kept until the tap dance is over to be able to release it right then.
uint16_t holdTapCode = 0;


//Handler for custom tap dances
void tap_custom( int kc ){ // enum customTap kc ){  //not restricted to customTaps to allow keydef codes
	switch( kc ){

		//────────────────── ASCII KEYS ──────────────────
		case SIM:	
		case DIFF:
		case QUOT_R:
		case GREQ:
		case QUOT_L:
		case LSEQ:	
			tap_ascii_key( kc );
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
			SEND_STRING( STRING_1 );
			break;
		case STR2:
			SEND_STRING( STRING_2 );
			break;		

		//────────────────── DEFAULT ──────────────────	
		default:
			break;	
	}
}



//Main tap dance event handler. According to the tap type and the tap count
//different actions are performed.
void dance_finished(qk_tap_dance_state_t *state, void *user_data) {
		tapdance td = tds[ ((tap_data*)user_data)->keycode ];
		uint16_t kc = state->count == 1 ? td.kc1 : td.kc2;
		switch(td.type){
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

//Vowels can be accented if pressed twice
void accent_each(qk_tap_dance_state_t *state, void *user_data) {
	uint16_t vowel = tds[ ((tap_data*)user_data)->keycode ].kc1;
	if(state->count==1){
		tap_code16( vowel );
	}else{
		tap_code16( KC_BSPC );
		tap_code16( ES_ACUT );
		tap_code16( vowel );
		state->finished = true;	
	}
}


//Handler hooks. Add new tap dances here to be able to use them in the keymap.c file using TD( keycode )
qk_tap_dance_action_t tap_dance_actions[] = {

	[ARROWS]  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   ARROWS  })),
	[COMMNT]  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   COMMNT  })),
	[LINES]   = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   LINES   })),
	[RSET]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   RSET    })),
	[TAG_O]   = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   TAG_O   })),  
	[TAG_C]   = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   TAG_C   })), 
	[DSK1]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DSK1    })), 
	[DSK2]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DSK2    })), 
	[DSK3]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   DSK3    })), 
	[ORD]     = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   ORD     })), 
	[SCR]     = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   SCR     })),     
	[EXIT]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   EXIT    })),    
	[GRTR]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   GRTR    })),    
	[LESS]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   LESS    })),     
	[NTEQ]    = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   NTEQ    })),   
	[GRTP]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   GRTP 	  })),    
	[LSTP]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   LSTP    })),
	[STR12]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  NULL, dance_finished, dance_reset,  &((tap_data){   STR12   })),
	[AC_A]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each, NULL, NULL,  &((tap_data){ AC_A })),
	[AC_E]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each, NULL, NULL,  &((tap_data){ AC_E })),
	[AC_I]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each, NULL, NULL,  &((tap_data){ AC_I })),
	[AC_O]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each, NULL, NULL,  &((tap_data){ AC_O })),
	[AC_U]	  = ACTION_TAP_DANCE_FN_ADVANCED_USER(  accent_each, NULL, NULL,  &((tap_data){ AC_U })),

};