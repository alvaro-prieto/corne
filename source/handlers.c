//#include QMK_KEYBOARD_H
#include "handlers.h"
#include "keydefs.h"
#include "dances.h"
#include "rgb.h"

//TO-DO: Sospecho que ya no lo necesito
extern uint8_t is_master;

//(OSX / WINDOWS) this variable is used to stablish the host OS. OSX by default
enum OS os = OSX;


//═══════════════════════════════════════════════════════════════
//  DEBUGGING FUNCTIONS
//═══════════════════════════════════════════════════════════════

//Comment this section in a release version. It is used only for debugging purposes
//Debugging function to visualize the active mod mask. The mod mask allow us to
//have virtual layers activated by overlapping several mod keys simultaneously
/*
void logMask( int n, int length){
  int c, k;
  for (c = length; c >= 0; c--){
  k = n >> c;
  if (k & 1)
	uprintf("1");
  else
	uprintf("0");
  }
  uprintf("\n");
};
*/

//═══════════════════════════════════════════════════════════════

//Returns Command / Control depending on the OS
uint16_t os_cmd_key(void){;
  return (  os == OSX ? KC_LCMD: KC_LCTRL );
}

//starts the CMD+TAB / ALT+TAB app switcher
void app_switch_start( void ){
  if(!app_switch){
	app_switch = true;
	register_code16( os == OSX ? KC_LCMD: KC_LOPT );
  }
}

//ends the CMD+TAB / ALT+TAB app switcher
void app_switch_end( void ){
  if(app_switch){
	app_switch = false;
	unregister_code16( os == OSX ? KC_LCMD: KC_LOPT );
  }
}

//fired when tilde mod is pressed
bool tildeDownHandler( Keypress *kp ){
    if(!delete){
        accent = true;
    }
    return true;
}


//fired when right shift is pressed
bool shiftDownHandler( Keypress *kp ){
    set_caps(true, false, false);
    return true;
}

//fired when right shift is released
void shiftUpHandler( Keypress *kp ){
    set_caps(false, false, false);
}

//fired when M1 is pressed
bool M1DownHandler ( Keypress *kp ){
  passive_down( PASSIVE_SHIFT );
  return true;
}

//fired when M1 is released
void M1UpHandler ( Keypress *kp ){
  passive_up();
}

//fired when M2 is released
void M2UpHandler ( Keypress *kp ){
  app_switch_end();
}

//fired when M6 is pressed
bool M6DownHandler ( Keypress *kp ){
  if(modMask == (S1_M + M6_M) ){
	interrupt_mods();
    set_caps(!caps_lock, true, false);
	return false;
  }
  return true;
}

//fired when control/window is pressed
bool ctrDownHandler( Keypress *kp ){
  if( os == OSX ){
	register_code(KC_LCTRL);
  }else{
	win = 1;
  }
  return true;
}

//fired when control/window is released
void ctrUpHandler( Keypress *kp ){
  if( os == OSX ){
	unregister_code(KC_LCTRL);
  }else{
	if(win == 2){
	  unregister_code(KC_LWIN);
	}
	win = 0;
  }
}



//internal function that toggles caps_state using only KC_CAPS instead of
//shift to prevent inteferences with other symbols, tildes, etc.
void set_caps_state(bool enabled, bool lock, bool word){
    if(caps_state != enabled) {
        //for whatever reason TAP_HOLD_CAPS_DELAY is not working for me in macOS
        register_code(KC_CAPS); wait_ms(110); unregister_code(KC_CAPS);
    }
	caps_state = enabled;
	#ifdef RGB_MATRIX_ENABLE
        if(lock){
            set_rgb_indicator(RGB_CAPS_WORD, false);
	        set_rgb_indicator(RGB_CAPS_LOCK, enabled);
        }
        if(word){
            set_rgb_indicator(RGB_CAPS_LOCK, false);
            set_rgb_indicator(RGB_CAPS_WORD, enabled);
        }
	#endif
}

//centralization of caps management (shift, caps lock and caps word)
void set_caps(bool enabled, bool lock, bool word){
    if(lock){
        //caps lock
        caps_lock = enabled;
        caps_word = shift = false;
    }else if(word){
        //caps word
        caps_lock = shift = false;
        caps_word = enabled;
    }else{
        //shift
        if(caps_lock || caps_word) return;
        shift = enabled;
    }
    set_caps_state(enabled, lock, word);
}

/*

   ____________
  [  MOD KEYS  ]
   ¯¯¯¯¯¯¯¯¯¯¯¯

  - enum modifier mod;    (Identifier, it has no relevant purpose)
  - short mask;       (to easily detect when several mods are hold simultaneously)
  - uint8_t layer;    (layer that is activated as soon as the modifier is active)
  - uint16_t skipTime;    (miliseconds to skip the secondary keycode if the hold is too long)
  - uint16_t keycode;   (keycode fired as soon as the mod is hold)
  - uint16_t alternativeKeycode;    (fired as alternative keycode if mod is released with no interruptions)
  - uint16_t osKeycode;         (OS dependent keycode fired as soon as the mod is hold)
  - uint16_t alternativeOsKeycode;    (fired as alternative OS dependent keycode if mod is released with no interruptions)
  - bool (*downHandler)( struct Keypress *);  (optional callback to be fired when the key is pressed. If returns false, mod is prevented)
  - void (*upHandler)( struct Keypress *);  (optional callback to be fired when the key is released)

  ─────────────────────────────────────────────────────────────────────────────────────

  - bool down;        (it is true when it is hold)
  - bool interrupted;   (it is true when other key has been pressed in conjunction)
  - uint16_t time;              (time when the first down event happened)
  - struct Keypress *next;      (pointer to another active mod key, to track them)

*/


//Mod and hold keys definition. Order dependent (keydefs.h)

Keypress KP[] = {

//COLOR           NAME        MASK    LAYER   SKIP_TIME KC        ALT_KC      OS_KC     ALT_OS_KC DOWN_CBACK          UP_CBACK          INTERNAL
//─────────────╂───────────╂───────╂──────╂────────╂────────╂──────────╂────────╂────────╂─────────────────╂────────────────╂───────────
/*  PINK    */  { M1_MOD,     M1_M,   _M1,    300,      0,        UNDERSC,    0,        0,        &M1DownHandler,     &M1UpHandler,     0, 0, 0, 0 },
/*  PURPLE  */  { M2_MOD,     M2_M,   _M2,    300,      0,        ES_SCLN,    0,        0,        0,                  &M2UpHandler,     0, 0, 0, 0 },
/*  RED     */  { M3_MOD,     M3_M,   _M3,    300,      0,        ES_EQL,     0,        0,        0,                  0,                0, 0, 0, 0 },
/*  GREEN   */  { M4_MOD,     M4_M,   _M4,    300,      0,        ES_COLN,    0,        0,        0,                  0,                0, 0, 0, 0 },
/*  BLUE    */  { M5_MOD,     M5_M,   _M5,    300,      0,        ES_DQUO,    0,        0,        0,                  0,                0, 0, 0, 0 },
/*  LIME    */  { M6_MOD,     M6_M,   _M6,    300,      0,        ES_QUOT,    0,        0,        &M6DownHandler,     0,                0, 0, 0, 0 },
/*  YELLOW  */  { M7_MOD,     NO_M,   _M7,    300,      0,        ES_PLUS,    0,        0,        0,                  0,                0, 0, 0, 0 },
/*  GREY    */  { CMD_MOD,    NO_M,   _MOD,   250,      0,        ES_MINS,    CMD_OS,   0,        0,                  0,                0, 0, 0, 0 },
/*  GREY    */  { ALT_MOD,    NO_M,   _MOD,   250,      KC_LALT,  KC_TAB,     0,        0,        0,                  0,                0, 0, 0, 0 },
/*  GREY    */  { CTR_MOD,    NO_M,   _MOD,   300,      0,        BCKQT,      0,        0,        &ctrDownHandler,    &ctrUpHandler,    0, 0, 0, 0 },
/*  SKY     */  { S1_MOD,     S1_M,   _BASE,  250,      0,        KC_ENT,     0,        0,        &shiftDownHandler,  &shiftUpHandler,  0, 0, 0, 0 },
/*  ORANGE  */  { TILDE_MOD,  NO_M,   _BASE,  250,      0,        0,          0,        0,        &tildeDownHandler,  0,                0, 0, 0, 0 },
};


//Resets the state of a mod/hold keypress
void resetState( Keypress *kp ){
  kp->down = false;
  kp->interrupted = false;
  kp->next = 0;
}

//If a layer is not being used by any hold/mod, then it is deactivated
void layer_off_if_not_used( char layer){
  if(layer){
	Keypress *kp = activeMods;
	uint8_t iteration = 0;
	while( kp != 0 && iteration++ < OVERFLOW_SMALL){
	  if( kp->layer == layer  ) return;
	  kp = kp->next;
	}
	layer_off( layer );
	//uprintf("Capa desactivada %d \n", layer);
  }
}

//Interrupts the mods queue (to prevent its alternative output)
void interrupt_mods( void ){
  Keypress *kp = activeMods;
  uint8_t iteration = 0;
  while( kp != 0 && iteration++ < OVERFLOW_SMALL){
	kp->interrupted = true;
	kp = kp->next;
  }
//TO-DO quitar esto cuando estemos conformes
    if(iteration >= OVERFLOW_SMALL){
       // uprintf("Overflow! evitarlo! \n");
    }

}


//Hold down a key in conjunction with the Apple FN key
void register_fn_key( uint16_t key){
  register_code16( KC_APPLE_FN );
  register_code16( key );
}

//Releases a key in conjunction with the Apple FN key
void unregister_fn_key( uint16_t key){
  unregister_code16( key );
  unregister_code16( KC_APPLE_FN );
}

//Tap a function key (F1, F2, F3...)
void tap_function_key( int fnNumber ){
  register_code16( KC_APPLE_FN );
  tap_code16( KC_F1 + fnNumber - 1);
  unregister_code16( KC_APPLE_FN );
}

//Hold down a function key (F1, F2, F3...)
void register_function_key( int fnNumber ){
  register_fn_key( KC_F1 + fnNumber - 1 );
}

//Releases a function key (F1, F2, F3...)
void unregister_function_key( int fnNumber ){
  unregister_fn_key( KC_F1 + fnNumber - 1 );
}

//It allows to tap an extended keycode in conjunction with the Apple FN key
void tap_fn_key(uint16_t keycode) {
  register_code(KC_APPLE_FN);
  tap_code16(keycode);
  unregister_code(KC_APPLE_FN);
}

//Hold down an OS dependent key
void register_os_dependent_key( uint16_t key ){
  uint16_t code = getOSKey( key );
  if( code ) register_code16( code );
}

//Releases an OS dependent key
void unregister_os_dependent_key( uint16_t key ){
  uint16_t code = getOSKey( key );
  if( code ) unregister_code16( code );
}

//Tap an OS dependent key
void tap_os_dependent_key( uint16_t key ){
  uint16_t code = getOSKey( key );
  if( code ) tap_code16( code );
}

//Smart tap a key according to its type. (This function can be improved to work with more types)
//Currently working for: normal keys + os keys + alias keys
void tap_generic_key(  uint16_t key ){
    if(key > FIRST_OS_INDEX && key < LAST_OS_INDEX){
        tap_os_dependent_key( key );
    }else if(key > FIRST_ALIAS_INDEX && key < LAST_ALIAS_INDEX){
        tap_code16( getAliasKey(key) );
    }else{
        tap_code16( key );
    }
}

//Smart register a key according to its type. (This function can be improved to work with more types)
//Currently working for: normal keys + os keys
void register_generic_key(  uint16_t key ){
    if(key > FIRST_OS_INDEX && key < LAST_OS_INDEX){
        register_os_dependent_key( key );
    }else{
        register_code16( key );
    }
}

//Smart unregister a key according to its type. (This function can be improved to work with more types)
//Currently working for: normal keys + os keys
void unregister_generic_key(  uint16_t key ){
    if(key > FIRST_OS_INDEX && key < LAST_OS_INDEX){
        unregister_os_dependent_key( key );
    }else{
        unregister_code16( key );
    }
}


//enables the mod layer
void enable_mod_layer( void ){
  layer_on(_MOD);
}

//disables the mod layer
void disable_mod_layer( void ){
  layer_off(_MOD);
}

//Hold down a passive key (a modifier that should not behave as modifier)
void passive_down( uint16_t key ){
  if( !passive ){
	register_code16( key );
	passive = key;
  }else{
	unregister_code16( passive );
	passive = 0;
  }
}

//Releases a passive key. It is called automatically in every new key down event
void passive_up( void ){
  if( passive ){
	unregister_code16( passive );
	passive = 0;
  }
}

/*
//Tap a shifted code (not needed in the current implementation)
void tap_shifted_code16( uint16_t key ){
    uint8_t mod_state = get_mods();
    del_mods(MOD_MASK_SHIFT);
    register_code16( KC_RSFT );
    tap_code16( key );
    unregister_code16( KC_RSFT );
    set_mods(mod_state);
}
*/


//═══════════════════════════════════════════════════════════════
//   BEFORE KEY PROCESSING
//═══════════════════════════════════════════════════════════════

//Function that gets called before processing any key event. It can perform several assertions,
//processing special cases, etc. Returns a boolean indicating wether the current key event should
//be processed or skipped (false = skip)
uint8_t klog1 = 0, klog2 = 0;
bool before_key_handler(uint16_t keycode, bool down, keyrecord_t *record){

  //New keypress means that old passive keys are invalidated
  passive_up();

  //Lazy mod. The windows key is not activated until a second key is pressed.
  if( down && os == WINDOWS && win==1 ){
	register_code( KC_LWIN );
	win = 2;
  }
  if( kb_lock ){
	if(down){
	  klog1 = klog2;
	  klog2 = keycode;
	  //unlock by typing: "yo"
	  if( klog1 == ES_Y && klog2 == ES_O ){
		kb_lock = false;
		layer_move( _BASE );
		#ifdef RGB_MATRIX_ENABLE
		  rgb_matrix_suspend_state_changed( false );
		  set_rgb_notification( RGB_LOCK_NOTIFICATION );
		#endif
	  }
	}
	return false;
  }

  return true;
}

//If there is a pending accent and a new keystroke is registered, its output depends on the
//the key itself according to some simple rules. Returns a boolean indicating wether the current
//key event should be processed or skipped (false = skip). Tilde key can also be used with othe purposes,
//such as entering in delete mode with double tap or followed by certain keys.
bool process_accents(uint16_t keycode, bool down, keyrecord_t *record){
    bool abort_caps_word = false;
    // Delete mode
    if(delete){
		if(down){
			if(keycode == delete){
                register_generic_key( registered_delete );
			}else{
                unregister_generic_key( registered_delete );
                delete = 0;
                return true;
            }
		}else{
			if(keycode == delete){
                unregister_generic_key( registered_delete );
			}else if( keycode > FIRST_MODIFIER_INDEX && keycode < LAST_MODIFIER_INDEX){
                mod_key_handler(keycode, down, record);
            }
		}
        return false;

    // Accent mode
    }else{

        //TO-DO, tengo que invocar a esto en algún sitio?
        //mod_key_handler(keycode, down, record);

        //modifiers should be interrupted
        interrupt_mods();

        //shift should not be prevented
        if(keycode == S1){
            return true;
            //abort_caps_word = true;
        }

        if(!down){
            //to prevent accents in scenarios like: i down, tilde down, (I UP), o down
            return true;
        }

        accent = false;
        //layer_move( _BASE );  //creo que no hace falta, borrarlo si se puede
        switch(keycode){

            // ´ Spanish accent
            case KC_A:
            case KC_E:
            case KC_I:
            case KC_O:
            case KC_U:
                tap_code16( ES_ACUT );
                tap_code16( keycode );
            break;

            // ' English apostrophe
            case KC_M:
            case KC_L:
            case KC_D:
            case KC_V:
            case KC_S:
            case KC_R:
            case KC_T:
            case KC_C:
                tap_code16( ES_QUOT );
                tap_code16( keycode );
                abort_caps_word = true;
            break;

            // DELETE MODE: double tap enters in delete mode
            case TILDE:
                //you can asign different kind of deletes to different keys (forward, word, line...)
                //remember to store the current key and current deletion mode in the following variables
                delete = TILDE;
                registered_delete = DEL_W; //KC_BSPC, DEL_W, KC_DEL, ...
                register_generic_key( registered_delete );
                abort_caps_word = true;
            break;

            // Ñ Spanish ñ
            case KC_N:
                tap_code16( ES_NTIL );
            break;

            // GÜ Spanish dieresis
            case KC_G:
                tap_code16( KC_G );
                tap_code16( ES_DIAE );
            break;

            // $ Dollar, common symbol for some programmers
            case KC_SPC:
                tap_code16( ES_DLR );
            break;

            // ; End of line, common combination for some programmers
            case M3:
                tap_code16( ES_SCLN );
                tap_code16( KC_ENT );
                abort_caps_word = true;
            break;

            // Caps word (enable / disable)
            case M2:
                if(caps_word){
                    abort_caps_word = true;
                }else{
                    set_caps(true, false, true);
                }

            break;

            /*
            case M3:
            case M4:
                delete = keycode;
                registered_delete = keycode == M3 ? DEL_W : KC_DEL;
				register_generic_key( registered_delete );
            break;
            */
        }

        if(caps_word && abort_caps_word ) set_caps(false, false, true);
        return false;

    }

}

//Smart caps to write words like MAXIMUM_DAMAGE, useful for programmers. Idea: @getreuer
bool process_caps_word(uint16_t keycode, bool down, keyrecord_t *record) {
    bool prevent_keypress = true;
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case ES_NTIL:

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
		case ES_MINS:
        case KC_BSPC:
        case KC_DEL:
        case ES_UNDS:
            if(down){
                tap_code16( keycode );
            }
            break;

        case CMD:
            if(down){
                tap_code16( ES_MINS );
            }
            break;

        case M1:
            if(down){
                tap_code16( ES_UNDS );
            }
            break;

        case M2:
        case TILDE:
            prevent_keypress = false;
            break;

        default: // Deactivate Caps Word.
            prevent_keypress = false;
            if(down){
                set_caps(false, false, true);
            }
            break;
    }
    return !prevent_keypress;
}

//Function that gets called whenever a new mod key is hold. It checks for custom mod combinations
//to perform special actions, such as swapping to the adjust layer
bool mod_combo( bool down ){
  //logMask( modMask, 7);
  enum rgb_indicator indicator = RGB_NONE;

  if(down){
	switch(modMask){
	  case M2_M + S1_M:
		interrupt_mods();
		shiftUpHandler(0);
		//uprintf("Hyper \n");
		indicator = RGB_HYPER_HOLD;
		hyper(true);
	  break;
	  case M1_M + M4_M:
		interrupt_mods();
		M1UpHandler(0);
		indicator = RGB_EXTRAS;
		//uprintf("Extras \n");
		layer_on(_CFG);
	  break;
	}

  }else{
	switch(lastMask){
	  case M2_M + S1_M:
		indicator = RGB_HYPER_HOLD;
		if(!hyper_lock){
		  hyper(false);
		}
	  break;
	  case M1_M + M4_M:
		//uprintf("Bye Extras \n");
		indicator = RGB_EXTRAS;
		layer_off(_CFG);
	  break;
	}
  }
  if(indicator != RGB_NONE){
	#ifdef RGB_MATRIX_ENABLE
	  set_rgb_indicator(indicator, down);
	#endif
  }

  lastMask = modMask;
  return false;

}



//═══════════════════════════════════════════════════════════════
//   KEY EVENT HANDLER
//═══════════════════════════════════════════════════════════════

/*
  Whenever a key is pressed / released, this function is called.
  Keycode could be an uint8_t / uint16_t code, depending on whether it is
  a basic or extended code. Extedend codes can come preboiled with
  modifiers, such as S(KC_A) which is Shift + A. Custom keycodes in this
  keymap are all extended.  The record structure holds information about the
  keystroke as follows:

  keyrecord_t record {
	keyevent_t event {
	  keypos_t key {
		uint8_t col
		uint8_t row
	  }
	  bool     pressed
	  uint16_t time
	}
  }
*/


bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  bool down = record->event.pressed;
  if( !before_key_handler(keycode, down, record) ) return false;
  if( (accent || delete) && !process_accents(keycode, down, record) ) return false;
  if( caps_word && !process_caps_word(keycode, down, record) ) return false;



  //In this switch we are going to group in categories different keys, such as standar keys,
  //mod keys, sequence keys, os keys, tap dances, custom managed keys, etc.
  switch (keycode) {

	//────────────────── MODIFIERS ──────────────────
	case FIRST_MODIFIER_INDEX ... LAST_MODIFIER_INDEX:
	  mod_key_handler(keycode, down, record);
	  mod_combo( down );
	  return false;

	//────────────────── APPLE FUNCTION KEYS ──────────────────
	case APPLE_F1 ... APPLE_F12:
	  if(down){
		interrupt_mods();
		tap_function_key( keycode - APPLE_F1 +1 );
	  }
	  return false;

	//────────────────── SEQUENCE KEYS ──────────────────
	case FIRST_SEQUENCE_INDEX ... LAST_SEQUENCE_INDEX:
	  if(down){
		interrupt_mods();
		tap_sequence( keycode );
	  }
	  return false;

	//────────────────── OS DEPENDENT KEYS ──────────────────
	case FIRST_OS_INDEX ... LAST_OS_INDEX:
	  if(down){
		interrupt_mods();
		register_os_dependent_key( keycode );
	  }else{
		unregister_os_dependent_key( keycode );
	  }
	  return false;

	//──────────────────BASIC KEYS NO MODS ──────────────────
	case KC_END:
	case KC_HOME:
	case KC_PGDN:
	case KC_PGUP:
	case ES_1 ... ES_0:
	  if(down){
		interrupt_mods();
		register_code16( keycode );
	  }else{
		unregister_code16( keycode );
	  }
	  return false;

	//────────────────── UNICODE KEYS ──────────────────
	case FIRST_UNICODE_INDEX ... LAST_UNICODE_INDEX:
	  if(down){
		interrupt_mods();
		tap_unicode_key( keycode );
	  }
	  return false;

	//────────────────── ALIAS KEYS ──────────────────
	case FIRST_ALIAS_INDEX ... LAST_ALIAS_INDEX:
  	  if(down){
		interrupt_mods();
		tap_code16( getAliasKey(keycode) );
	  }
    return false;



	//════════════════ CUSTOM HANDLED KEYS ══════════════

	case DEL:
	  if(down){
		if(shift){
		  interrupt_mods();
		  register_os_dependent_key( DEL_LN );
		}else{
		  register_code16( KC_BSPC );
		}
	  }else{
		if(shift){
		  unregister_os_dependent_key( DEL_LN );
		}else{
		  unregister_code16( KC_BSPC );
		}
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case DOT:
	  if(down){
		interrupt_mods();
		if(shift){
		  tap_unicode_key( BULLET );
		}else{
		  register_code16( ES_DOT );
		}
	  }else{
		if(!shift){
		  unregister_code16( ES_DOT );
		}
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case SWAP_OS:
	  if(down){
		interrupt_mods();
		if(os == OSX){
		  os = WINDOWS;
		  #ifdef RGB_MATRIX_ENABLE
			set_rgb_notification( RGB_WINDOWS_NOTIFICATION );
		  #endif
		}else{
		  os = OSX;
		  #ifdef RGB_MATRIX_ENABLE
			set_rgb_notification( RGB_MAC_NOTIFICATION );
		  #endif
		}

		#ifdef OLED_DRIVER_ENABLE
		  oled_os_notification( os );
		#endif
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case KBLOCK:
	  if(down){
		interrupt_mods();
	  }else{
		if(os == OSX){
		  tap_fn_key(C(S(KC_POWER)));
		}else{
		  tap_code16(KC_SLEP);
		}
		kb_lock = true;
		layer_move( _MOD );
		#ifdef RGB_MATRIX_ENABLE
			set_rgb_notification( RGB_LOCK_NOTIFICATION );
		#endif
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case OSLOCK:
	  if(down){
		interrupt_mods();
		register_os_dependent_key( LOCK );
	  }else{
		unregister_os_dependent_key( LOCK );
		#ifdef LOCK_KB_ON_LOCK_OS
			if(os == OSX){
				tap_fn_key(C(S(KC_POWER)));
			}else{
				tap_code16(KC_SLEP);
			}
			kb_lock = true;
			layer_move( _MOD );
			#ifdef RGB_MATRIX_ENABLE
				set_rgb_notification( RGB_LOCK_NOTIFICATION );
			#endif
		  #endif
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case SHW_DSK:
	  if(down) interrupt_mods();
	  if(os == OSX){
		if(down){
		  register_function_key(11);
		}else{
		  unregister_function_key(11);
		}
	  }else{
		if(down){
		  tap_code16(LWIN(KC_D));
		}
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case EXPS:
	  if(down) interrupt_mods();
	  if(os == OSX){
		if(down){
		  register_code16(KC_F3);
		}else{
		  unregister_code16(KC_F3);
		}
	  }else{
		if(down){
		  tap_code16(LWIN(KC_TAB));
		}
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case SCR_OFF:
	  if(down){
		interrupt_mods();
	  }else{
		if(os == OSX){
		  tap_fn_key(C(S(KC_POWER)));
		}else{
		  tap_code16(KC_SLEP);
		}
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case NEXT_APP:
	  if(down){
		interrupt_mods();
		if(!app_switch){
		  app_switch_start();
		}
		register_code(KC_TAB);
	  }else{
		unregister_code(KC_TAB);
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case PREV_APP:
	  if(down){
		interrupt_mods();
		if(!app_switch){
		  app_switch_start();
		}
		register_code16( S(KC_TAB) );
	  }else{
		unregister_code16( S(KC_TAB) );
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·
    /*
    //Opens the boot options menu. Changed to a Tap Dance: turn off / reboot
	case BOOT:
	  if(down){
		interrupt_mods();
		if(os == OSX){
		  tap_code16( C(KC_POWER) );
		}else{
		  tap_code16( LWIN(KC_X) );
		  tap_code16( KC_G );
		}
	  }
	return false;
    */

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case H_LOCK:
	  if(down){
		interrupt_mods();
		toggle_hyper_lock();
	  }

	return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·

	case RGB_PREV:
	case RGB_NEXT:
	  if(down){
		interrupt_mods();
		#ifdef RGB_MATRIX_ENABLE
		rgb_rotate_theme(keycode == RGB_NEXT ? 1 : -1);
		#endif
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·
	case RGB_UP:
	case RGB_DOWN:
	  if(down){
		interrupt_mods();
		#ifdef RGB_MATRIX_ENABLE
		rgb_brightness(keycode == RGB_UP ? 1 : -1);
		#endif
	  }
	  return false;

	// · · · · · · · · · · · · · · · · · · · · · · · · ·
	case NLOCK:
	  if(down){
		interrupt_mods();
        num_lock = !num_lock;
        layer_move( num_lock ? _NL : _BASE );
		#ifdef RGB_MATRIX_ENABLE
            set_rgb_indicator(RGB_NUM_LOCK, num_lock);
		#endif
	  }
	  return false;


    // · · · · · · · · · · · · · · · · · · · · · · · · ·

    //not needed in the current implementation
    /*
	case LNG:
	  if(down){
		interrupt_mods();
		lang = ( lang == ES ? EN : ES );
		#ifdef RGB_MATRIX_ENABLE
		  set_rgb_notification( lang == ES ? RGB_LANG_ES : RGB_LANG_EN );
		#endif
	  }
	  return false;
    */

	//────────────────── DEFAULT ──────────────────

	default:
	  if(modsNumber > 0 && !IS_LAYER_ON(_HYP) ){
		interrupt_mods();
	  }

	  //the following lines could be skipped, but then, holding down combined keycodes
	  //could contamine the following ones if not fully released. For example: %+3 = %·
	  //Detect shifted keycodes:
	  if((keycode >= QK_LSFT && keycode <= (QK_LSFT + 0xFF)) ||  (keycode >= QK_RSFT && keycode <= (QK_RSFT + 0xFF))){
		  //avoid hold down shifted keycodes to prevent contamination for fast typist
		  if(down){
			tap_code16( keycode );
		  }
		  return false;
	  }


  }

  return true;
}





//═══════════════════════════════════════════════════════════════
//   MOD KEY HANDLER
//═══════════════════════════════════════════════════════════════

//Handler for 'mod keys'. Mod keys could behave slightly differently according to the mod itself,
//but in general they can activate a temporary layer as far as they are hold, making it possible
//to modify the default behaviour of any key. Mods can also hold any keycode as soon as they get pressed,
//(such as CMD, or ALT), and also can output a secondary keycode if tapped alone (with no other key being
//pressed between press and release). This behaviour allows a very wide variety of mods, such as
//spacebar that if hold down behaves like a CMD and even redefining some keys by swapping to another layer.

bool mod_key_handler(uint16_t keycode, bool down, keyrecord_t *record){
  bool reset = false;
  Keypress *currentMod = NULL;
  Keypress *kp = &KP[ keycode - FIRST_MODIFIER_INDEX - 1];
  uint8_t iteration = 0;

  //────────────────── MOD DOWN ──────────────────
  if(down){
	//uprintf("Mod down %d\n", keycode - FIRST_MODIFIER_INDEX - 1);
	modMask += kp->mask;
	//logMask( modMask, 7);
	if(kp->downHandler){
	  //uprintf("Custom handler found \n");
	  if( !((*kp->downHandler)( kp )) ){
		//uprintf("Prevented MOD\n");
		modMask -= kp->mask;
		return false;
	  }
	}
	//uprintf("MOD init time is stored\n");
	kp->time = record->event.time;
	if(kp->layer) layer_on(kp->layer);
	if(kp->keycode || kp->osKeycode){
	  register_code16( kp->osKeycode ? getOSKey(kp->osKeycode) : kp->keycode);
	}
	if(modsNumber++ == 0){
	  activeMods = kp;
	}else{
	  currentMod = activeMods;
	  iteration = 0;
	  while(currentMod->next != 0 && iteration++ < OVERFLOW_SMALL){
		currentMod = currentMod->next;
	  }
	  currentMod->next = kp;
	  interrupt_mods();
	}

    //if there is a pending accent, no alternative output is required
    if(accent){
        kp->interrupted = true;
    }

  //────────────────── MOD UP ──────────────────
  }else{
	//uprintf("Mod up %d\n", keycode - FIRST_MODIFIER_INDEX - 1);
	//since mods can be prevented by its down handler, check if it is active
	if( kp->mask && !(modMask & kp->mask) ) return false;

	//keep the activeMods queue updated
    if(modsNumber <= 0 || !mod_is_active(kp)){
        //in some implementations and scenarios, it could happen that a mod
        //is prevented in its activation (down) and allowed in its release (up)
        reset = true;
    }else if(--modsNumber == 0){
	  activeMods = NULL;
	  reset = true;
	  //uprintf("No remaining mods\n");
	}else{
	  //uprintf("Remaining mods\n");
	  if(activeMods == kp){
		reset = true;
		activeMods = activeMods->next;
	  }else{
		currentMod = activeMods;
		iteration = 0;
		while( currentMod->next != 0 && iteration++ < OVERFLOW_SMALL){
		  if( currentMod->next == kp){
			currentMod->next = currentMod->next->next;
			resetState( kp );
			break;
		  }
		  currentMod = currentMod->next;
		}
	  }
	}

	//return to its previus state (layer, keyup, ...)
	modMask -= kp->mask;
	if(kp->upHandler){ (*kp->upHandler)( kp ); }
	if(kp->layer){
	  //uprintf("Mod has its own layer, lets try to deactivate it %d\n", kp->layer);
	  layer_off_if_not_used(kp->layer);
	}

	if(kp->keycode || kp->osKeycode){
	  unregister_code16( kp->osKeycode ? getOSKey(kp->osKeycode) : kp->keycode);
	}

	//If the mod key has not been interrupted, then its second behaviour is activated
	//uprintf("MOD has an alternative key?\n");
	if( !kp->interrupted ){
	  //uprintf("it has not been interrupted\n");
	  if( !kp->skipTime || (record->event.time - kp->time) < kp->skipTime  ){
		//uprintf("timing is OK, go ahead with the alternative keycode\n");
        bool abort_caps_word = true;
		if( kp->alternativeKeycode && !(accent || delete) ){
			switch( kp->alternativeKeycode ) {
				//used key sequences goes here:
				case BCKQT:
					tap_sequence( kp->alternativeKeycode );
				break;
                //underscore doesn't abort the caps_word mode
                case UNDERSC:
                    abort_caps_word = false;
                    tap_generic_key( kp->alternativeKeycode );
                break;
				//standar alternative keycodes
				default:
					tap_generic_key( kp->alternativeKeycode );
			}
		}
		if( kp->alternativeOsKeycode ){
            tap_os_dependent_key( kp->alternativeOsKeycode );
        }
        if(caps_word && abort_caps_word ) set_caps(false, false, true);
	  }
	}
	if( reset ){
	  resetState( kp );
	}
  }
  return false;
}

//this function tell us if a certain mod key is in the activeMods queue
bool mod_is_active(Keypress *kp){
    Keypress *currentMod = activeMods;
	uint8_t iteration = 0;
    if(currentMod == NULL) return false;
    while( currentMod != NULL && iteration++ < OVERFLOW_SMALL){
        if( currentMod == kp){
            return true;
        }
        currentMod = currentMod->next;
    }
    return false;
}

//═══════════════════════════════════════════════════════════════
//   HYPER
//═══════════════════════════════════════════════════════════════

//Activates / deactivates hyper mode according to the down parameter
void hyper( bool down){
	interrupt_mods();
	if( down ){
		layer_move( _HYP );
		register_code16( KC_LSFT );
		register_code16( KC_LCTRL );
		register_code16( KC_LALT );
	}else{
		unregister_code16( KC_LSFT );
		unregister_code16( KC_LCTRL );
		unregister_code16( KC_LALT );
		layer_move( _BASE );
	}
}

//Toggle hyper lock
void toggle_hyper_lock(void){
  hyper_lock = !hyper_lock;
  hyper( hyper_lock );
  #ifdef RGB_MATRIX_ENABLE
  set_rgb_indicator(RGB_HYPER_LOCK, hyper_lock);
  #endif
}


//═══════════════════════════════════════════════════════════════
//   KEYBOARD INIT
//═══════════════════════════════════════════════════════════════


void keyboard_post_init_user(void) {

  #ifdef RGB_MATRIX_ENABLE
  init_rgb();
  #endif

  // Customise these values to desired behaviour
  //debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
/*
void keyboard_pre_init_user(void) {}
*/
