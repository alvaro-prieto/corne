#include "shared.h"
#include "sync.h"
#include "handlers.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═══════════════════════════════════════════════════════════════
//   CUSTOM MODIFIERS
//═══════════════════════════════════════════════════════════════

/*
   ____________
  [  MOD KEYS  ]
   ¯¯¯¯¯¯¯¯¯¯¯¯
  Mod and hold keys definition. Order dependent (keydefs.h)

  - enum modifier mod;  (Identifier, it has no relevant purpose)
  - short mask; 	    (to easily detect when several mods are hold simultaneously)
  - uint8_t layer;	    (layer that is activated as soon as the modifier is active)
  - uint16_t skipTime;  (miliseconds to skip the secondary keycode if the hold is too long)
  - uint16_t keycode;   (keycode fired as soon as the mod is hold)
  - uint16_t alternativeKeycode;    (fired as alternative keycode if mod is released with no interruptions)
  - uint16_t osKeycode;             (OS dependent keycode fired as soon as the mod is hold)
  - uint16_t alternativeOsKeycode;              (fired as alternative OS dependent keycode if mod is released with no interruptions)
  - bool (*downHandler)( struct Keypress *);    (optional callback to be fired when the key is pressed. If returns false, mod is prevented)
  - void (*upHandler)( struct Keypress *);      (optional callback to be fired when the key is released)

  ─────────────────────────────────────────────────────────────────────────────────────

  - bool down;          (it is true when it is hold)
  - bool interrupted;   (it is true when other key has been pressed in conjunction)
  - uint16_t time;      (time when the first down event happened)

*/
static Keypress KP[] = {

//COLOR 		  NAME		  MASK	   LAYER   SKIP_TIME   KC		 ALT_KC	     OS_KC 	   ALT_OS_KC  DOWN_CBACK		   UP_CBACK		 	 INTERNAL STATE
//─────────────╂───────────╂───────╂──────╂──────────╂────────╂──────────╂────────╂────────╂───────────────────╂────────────────╂────────────────
/*	💗 PINK    */  { M1_MOD,	 M1_M,	 _M1,	 300,	   0,		 UNDERSC,	 0, 	   0,	    &M1DownHandler,	    &M1UpHandler,       false, false, 0 },
/*	🟣 PURPLE  */  { M2_MOD,	 M2_M,	 _M2,	 300,	   0,		 ES_SCLN,	 0, 	   0,		0, 				    &M2UpHandler,       false, false, 0 },
/*	🔴 RED	   */  { M3_MOD,	 M3_M,	 _M3,	 300,	   0,		 ES_EQL,	 0, 	   0,		0, 				    0, 			        false, false, 0 },
/*	🟢 GREEN   */  { M4_MOD,	 M4_M,	 _M4,	 300,	   0,		 ES_COLN,	 0, 	   0,		0, 				    0, 		            false, false, 0 },
/*	🔵 BLUE    */  { M5_MOD,	 M5_M,	 _M5,	 300,	   0,		 ES_QUOT,	 0, 	   0,		0, 				    0, 			        false, false, 0 },
/*	🥝 LIME    */  { M6_MOD,	 M6_M,	 _M6,	 300,	   0,		 BCKQT, 	 0, 	   0,		&M6DownHandler,	    0, 			        false, false, 0 },
/*	🟡 YELLOW  */  { M7_MOD,	 NO_M,	 _M7,	 300,	   0,		 ES_PLUS,	 0, 	   0,		0, 				    0, 			        false, false, 0 },
/*	🔠 GREY    */  { CMD_MOD,	 NO_M,	 _MOD,	 250,	   0,		 ES_MINS,	 CMD_OS,   0,		&cmdDownHandler,    0, 			        false, false, 0 },
/*	🔠 GREY    */  { ALT_MOD,	 NO_M,	 _MOD,	 250,	   KC_LALT,  KC_TAB,	 0, 	   0,		0, 				    0, 			        false, false, 0 },
/*	🔠 GREY    */  { CTR_MOD,	 NO_M,	 _MOD,	 300,	   0,		 0, 		 0, 	   MUTE,	&ctrDownHandler,    &ctrUpHandler,      false, false, 0 },
/*	💎 SHIFT   */  { S1_MOD,	 S1_M,	 _BASE,  400,	   0,		 KC_ENT,	 0, 	   0,		&shiftDownHandler,  &shiftUpHandler,    false, false, 0 },
/*	🟠 ORANGE  */  { TILDE_MOD,  NO_M,	 _BASE,  250,	   0,		 0, 		 0, 	   0,		&tildeDownHandler,  0, 			        false, false, 0 },
};

// Maximum number of modifiers is calculated at compile time.
#define MAX_MODIFIERS (sizeof(KP) / sizeof(Keypress))

// Static array to store pointers to active modifiers.
static Keypress* active_mods[MAX_MODIFIERS] = {0};


// Resets the internal state of a modifier for its next use.
static void resetState( Keypress *kp ){
  if (kp) {
    kp->down = false;
    kp->interrupted = false;
  }
}

// Adds a modifier to the active list
static void add_active_mod(Keypress *kp) {
    for (int i = 0; i < MAX_MODIFIERS; i++) {
        if (active_mods[i] == NULL) {
            active_mods[i] = kp;
            return;
        }
    }
}

// Removes a modifier from the active list.
static void remove_active_mod(Keypress *kp) {
    for (int i = 0; i < MAX_MODIFIERS; i++) {
        if (active_mods[i] == kp) {
            active_mods[i] = NULL;
            return;
        }
    }
}

// Deactivates a layer if no active modifier is using it
static void layer_off_if_not_used( uint8_t layer ){
    if (layer == 0) return;
    for (int i = 0; i < MAX_MODIFIERS; i++) {
        if (active_mods[i] != NULL && active_mods[i]->layer == layer) {
            return; // layer is still in use by another modifier
        }
    }
    layer_off(layer);
}

// Interrupts all active modifiers (to prevent their secondary action).
void interrupt_mods(void){
    for (int i = 0; i < MAX_MODIFIERS; i++) {
        if (active_mods[i] != NULL) {
            active_mods[i]->interrupted = true;
        }
    }
}

// Hold down a passive key (a modifier that should not behave as modifier)
void passive_down( uint16_t key ){
  if( !passive ){
	register_code16( key );
	passive = key;
  }else{
	unregister_code16( passive );
	passive = 0;
  }
}

// Releases a passive key. It is called automatically in every new key down event
void passive_up( void ){
  if( passive ){
	unregister_code16( passive );
	passive = 0;
  }
}

//═══════════════════════════════════════════════════════════════
//	 MOD KEY HANDLER
//═══════════════════════════════════════════════════════════════
// Handler for 'mod keys'. Each mod could behave slightly differently according to its configuration,
// but in general they can activate a temporary layer as far as they are hold, making it possible
// to modify the default behaviour of any key. Mods can also hold any keycode as soon as they get pressed,
// (such as CMD, or ALT), and also can output a secondary keycode if tapped alone (with no other key being
// pressed between press and release). This behaviour allows a very wide variety of mods, such as
// spacebar that if hold down behaves like a CMD and even redefining some keys by swapping to another layer.

bool mod_key_handler(uint16_t keycode, bool down, keyrecord_t *record){
    if (keycode < FIRST_MODIFIER_INDEX || keycode > (FIRST_MODIFIER_INDEX + MAX_MODIFIERS)) {
        return false;
    }
    Keypress *kp = &KP[keycode - FIRST_MODIFIER_INDEX - 1];

    //────────────────── MOD DOWN ──────────────────
    if (down) {
        // If the key is already considered down, this is an anomalous event. Ignore to prevent inconsistencies.
        if (kp->down) return false;
        interrupt_mods();
        modMask += kp->mask;
        if (kp->downHandler) {
            if (!((*kp->downHandler)(kp))) {
                modMask -= kp->mask;
                return false;
            }
        }
        kp->time = record->event.time;
        kp->interrupted = false;
        kp->down = true;
        if (kp->layer) layer_on(kp->layer);
        if (kp->keycode || kp->osKeycode) {
            register_code16(kp->osKeycode ? getOSKey(kp->osKeycode) : kp->keycode);
        }
        add_active_mod(kp);
        // if there is a pending accent, no alternative output is required
        if (accent) {
            kp->interrupted = true;
        }

    //────────────────── MOD UP ──────────────────
    } else {
        if (!kp->down) return false;
        // log_mask( modMask, 7);
        remove_active_mod(kp);
        modMask -= kp->mask;
        if (kp->upHandler)
            (*kp->upHandler)(kp);
        if (kp->layer)
            layer_off_if_not_used(kp->layer);
        if (kp->keycode || kp->osKeycode)
            unregister_code16(kp->osKeycode ? getOSKey(kp->osKeycode) : kp->keycode);

        // If the mod key has not been interrupted, then its second behaviour is activated
        if (!kp->interrupted) {
            if (!kp->skipTime || (record->event.time - kp->time) < kp->skipTime) {
                bool abort_caps_word = true;
                if (kp->alternativeKeycode && !(accent || delete)) {
                    switch (kp->alternativeKeycode) {
                        case BCKQT:
                            tap_sequence(kp->alternativeKeycode);
                            break;
                        case UNDERSC:
                            abort_caps_word = false;
                            tap_generic_key(kp->alternativeKeycode);
                            break;
                        default:
                            tap_generic_key(kp->alternativeKeycode);
                    }
                }
                if (kp->alternativeOsKeycode) {
                    tap_os_dependent_key(kp->alternativeOsKeycode);
                }
                if (caps_word && abort_caps_word) set_caps(false, false, true);
            }
        }
        resetState(kp);
    }
    return false;
}


//═══════════════════════════════════════════════════════════════
//	 HYPER
//═══════════════════════════════════════════════════════════════

// Activates / deactivates hyper mode according to the down parameter
void hyper( bool down){
	if( down ){
		interrupt_mods();
		layer_move( _HYP );
		register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT) );
	}else{
		unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT) );
		layer_move( _BASE );
	}
}

// Toggle hyper lock
void toggle_hyper_lock(void){
	hyper_lock = !hyper_lock;
	hyper( hyper_lock );
	#ifdef RGB_MATRIX_ENABLE
	rgb_set_indicator(RGB_HYPER, hyper_lock);
	#endif
}

// Check simultaneous mod combinations to trigger custom actions
void trigger_mod_combo(uint8_t mask, bool down){
    // logMask( modMask, 7);
	#ifdef RGB_MATRIX_ENABLE
	enum rgb_indicator indicator = RGB_NONE;
	#endif
	if(down){
		switch(mask){
			case M2_M + S1_M:
				shiftUpHandler(0);
				#ifdef RGB_MATRIX_ENABLE
				indicator = RGB_HYPER_LOCK;
				#endif
				hyper(true);
			break;
			case M1_M + M4_M:
				M1UpHandler(0);
				#ifdef RGB_MATRIX_ENABLE
				indicator = RGB_EXTRAS;
				#endif
				layer_on(_CFG);
			break;
		}
	}else{
		switch(mask){
			case M2_M + S1_M:
				#ifdef RGB_MATRIX_ENABLE
				indicator = RGB_HYPER_LOCK;
				#endif
				if(!hyper_lock){
					hyper(false);
				}
			break;
			case M1_M + M4_M:
				#ifdef RGB_MATRIX_ENABLE
				indicator = RGB_EXTRAS;
				#endif
				layer_off(_CFG);
			break;
		}
	}
	#ifdef RGB_MATRIX_ENABLE
	if(indicator != RGB_NONE){
		rgb_set_indicator(indicator, down);
	}
	#endif
}

// Function that gets called whenever a new mod key is hold. It checks for custom mod combinations
// to perform special actions, such as swapping to the adjust layer
void mod_combo( bool down ){
	trigger_mod_combo(down ? modMask : lastMask, down);
	lastMask = modMask;
}


//═══════════════════════════════════════════════════════════════
//	 Hooks
//═══════════════════════════════════════════════════════════════

// Returns Command / Control depending on the OS
uint16_t os_cmd_key(void){;
  return (	os == OSX ? KC_LCMD: KC_LCTL );
}

// Fired when tilde mod is pressed
bool tildeDownHandler( Keypress *kp ){
	if(modMask == S1_M){
		tap_code16( S(KC_ENT) );
        kp->interrupted = true;
		interrupt_mods();
		return false;
	}
	if(!delete){
		accent = true;
	}
	return true;
}

// Fired when right shift (S1) is pressed
bool shiftDownHandler( Keypress *kp ){
	set_caps(true, false, false);
	return true;
}

// Fired when right shift (S1) is released
void shiftUpHandler( Keypress *kp ){
	set_caps(false, false, false);
}

// Fired when M1 is pressed
bool M1DownHandler ( Keypress *kp ){
  passive_down( KC_RSFT );
  return true;
}

// Fired when M1 is released
void M1UpHandler ( Keypress *kp ){
  passive_up();
}

// Fired when M2 is released
void M2UpHandler ( Keypress *kp ){
  app_switch_end();
}

// Fired when M6 is pressed
bool M6DownHandler ( Keypress *kp ){
  if(modMask == (S1_M + M6_M) ){
	interrupt_mods();
    kp->interrupted = true;
	set_caps(!caps_lock, true, false);
	return false;
  }
  return true;
}

// Fired when command/control is pressed
bool cmdDownHandler( Keypress *kp ){
	if(shift){
		interrupt_mods();
        kp->interrupted = true;
		tap_unicode_key( LONGLN );
		return false;
	}
	return true;
}

// Fired when control/window is pressed
bool ctrDownHandler( Keypress *kp ){
  if( os == OSX ){
	register_code(KC_LCTL);
  }else{
	win = 1;
  }
  return true;
}

// Fired when control/window is released
void ctrUpHandler( Keypress *kp ){
  if( os == OSX ){
	unregister_code(KC_LCTL);
  }else{
	if(win == 2){
	  unregister_code(KC_LWIN);
	}
	win = 0;
  }
}


//═══════════════════════════════════════════════════════════════
//	DEBUGGING FUNCTIONS
//═══════════════════════════════════════════════════════════════

#ifdef CONSOLE_ENABLE
// Debugging function to display the active mod mask as a binary string.
// The mod mask represents virtual layers activated by pressing multiple mod keys.
void log_mask( int n, int length){
  int c, k;
  for (c = length; c >= 0; c--){
  k = n >> c;
  if (k & 1)
	xprintf("1");
  else
	xprintf("0");
  }
  xprintf("\n");
};
#endif
