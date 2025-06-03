#include "shared.h"
#include "sync.h"
#include "handlers.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═══════════════════════════════════════════════════════════════
//   SMART HANDLERS  (smart accents, caps word, etc)
//═══════════════════════════════════════════════════════════════

// If there is a pending accent and a new keystroke is registered, its output depends on the
// the key itself according to some simple rules. Returns a boolean indicating wether the current
// key event should be processed or skipped (false = skip). Tilde key can also be used with othe purposes,
// such as entering in delete mode with double tap or followed by certain keys.

void handle_accents(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
    if(!(accent || delete)) return;
    bool abort_caps_word = false;
    // Delete mode
    if(delete){
		if(down){
			if(keycode == delete){
                register_generic_key( registered_delete );
			}else{
                unregister_generic_key( registered_delete );
                delete = 0;
                return;
            }
		}else{
			if(keycode == delete){
                unregister_generic_key( registered_delete );
			}else if( keycode > FIRST_MODIFIER_INDEX && keycode < LAST_MODIFIER_INDEX){
                mod_key_handler(keycode, down, record);
            }
		}
        *handled = true;
        *stop_propagation = true;
    // Accent mode
    }else{

        // TO-DO, tengo que invocar a esto en algún sitio? (parece que no)
        // mod_key_handler(keycode, down, record);

        // modifiers should be interrupted
        interrupt_mods();

        // shift should not be prevented
        if(keycode == S1){
            return;
            //abort_caps_word = true;
        }

        if(!down){
            // to prevent accents in scenarios like: i down, tilde down, (I UP), o down
            return;
        }

        accent = false;
        // layer_move( _BASE );  //creo que no hace falta, borrarlo si se puede
        switch(keycode){

            // ´ Spanish accent
            case KC_A:
            case KC_E:
            case KC_I:
            case KC_O:
            case KC_U:
                tap_unshifted( ES_ACUT );
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
                tap_unshifted( ES_QUOT );
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
            /*
            case KC_G:
                tap_code16( KC_G );
                tap_unshifted( ES_DIAE );
            break;
            */

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
            /*
            case M2:
                if(caps_word){
                    abort_caps_word = true;
                }else{
                    set_caps(true, false, true);
                }
            break;
            */

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
        *handled = true;
        *stop_propagation = true;

    }

}

// Smart caps to write words like MAXIMUM_DAMAGE or U.S.A. useful for programmers. Idea: @getreuer
void handle_caps_word(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
    if(!caps_word) return;
    bool prevent_keypress = true;
    switch (keycode) {
        case M2:
        case TILDE:
            prevent_keypress = false;
            break;

        default:
            if(down){
                switch (keycode) {
                    case KC_A ... KC_Z:
                    case ES_NTIL:
                    case KC_1 ... KC_0:
                    case ES_DOT:
                    case ES_MINS:
                    case KC_BSPC:
                    case KC_DEL:
                    case ES_UNDS:
                        tap_code16(keycode);
                        break;
                    case DEL:
                        tap_code16(KC_BSPC);
                        break;
                    case CMD:
                        tap_code16(ES_MINS);
                        break;
                    case M1:
                        tap_code16(ES_UNDS);
                        break;
                    default:
                        // Deactivate Caps Word.
                        set_caps(false, false, true);
                        prevent_keypress = false;
                        break;
                }
            }
            break;
    }

    if(prevent_keypress){
        *handled = true;
        *stop_propagation = true;
    }
}

