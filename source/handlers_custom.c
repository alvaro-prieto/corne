#include "shared.h"
#include "sync.h"
#include "handlers.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═══════════════════════════════════════════════════════════════
//  CUSTOM HANDLERS
//═══════════════════════════════════════════════════════════════


// Handler for testing purposes
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_test_key(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
    // ALT A,  CMD G,  CONTROL C, SHIFT S
	*handled = true;
	*stop_propagation = true;
    if(keycode == TEST){
        if(down){
            interrupt_mods();
            #ifdef RGB_MATRIX_ENABLE
            //rgb_set_notification( RGB_RING_NOTIFICATION );
            //rgb_set_animation( RGB_FIRE_ANIMATION );
            //rgb_rotate_animation( 1 );
             //rgb_rotate_reactive( 1 );
             send_string_sp(
                "¡Todo! ¿Funciona? 0123456789 () áéíóúÁÉÍÓÚñÑçÇüÜªº@€|¬#~[]{};:*/\\·"
            );
            #endif
        }
    }

    if(keycode == TEST2){
        if(down){
            interrupt_mods();
            #ifdef RGB_MATRIX_ENABLE
            //rgb_set_notification( RGB_RING_NOTIFICATION );
            //rgb_set_animation( RGB_FIRE_ANIMATION );
            rgb_rotate_animation( 1 );
            // rgb_rotate_reactive( 1 );
            #endif
        }
    }
}


void handle_custom(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	*handled = true;
	switch (keycode) {
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
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case SWAP_OS:
			if(down){
                #ifdef AUDIO_ENABLE
					float caps_on[][2] = SONG(CAPS_LOCK_ON_SOUND);
					PLAY_SONG( caps_on );
				#endif
				interrupt_mods();
				if(os == OSX){
				os = WINDOWS;
				#ifdef RGB_MATRIX_ENABLE
					rgb_set_notification( RGB_WINDOWS_NOTIFICATION );
				#endif
				}else{
				os = OSX;
				#ifdef RGB_MATRIX_ENABLE
					rgb_set_notification( RGB_MAC_NOTIFICATION );
				#endif
				}

				#ifdef OLED_DRIVER_ENABLE
				oled_os_notification( os );
				#endif

			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case KBLOCK:
			if(down){
				interrupt_mods();
			}else{
				(os == OSX)
                    ? tap_code16(C(S(KC_KB_POWER)))
                    : tap_code16(KC_SLEP);
				kb_lock = true;
				sync(SYNC_KB_LOCK, 1, 0);
				layer_move( _MOD );
				#ifdef RGB_MATRIX_ENABLE
					rgb_set_notification( RGB_LOCK_NOTIFICATION );
				#endif
			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case OSLOCK:
			if(down){
				interrupt_mods();
				register_os_dependent_key( LOCK );
			}else{
				unregister_os_dependent_key( LOCK );
				#if LOCK_KB_ON_LOCK_OS
					if(os == OSX){
                        tap_code16(C(S(KC_KB_POWER)));
					}else{
						tap_code16(KC_SLEP);
					}
					kb_lock = true;
					layer_move( _MOD );
					#ifdef RGB_MATRIX_ENABLE
						rgb_set_notification( RGB_LOCK_NOTIFICATION );
					#endif
				#endif
			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case SHW_DSK:
			if(down) interrupt_mods();
			if(os == OSX){
				if(down){
                    register_code16( KC_MISSION_CONTROL );
				//register_function_key(11);
				}else{
                    unregister_code16( KC_MISSION_CONTROL );
				//unregister_function_key(11);
				}
			}else{
				if(down){
				tap_code16(LWIN(KC_D));
				}
			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case EXPS:
			if(down) interrupt_mods();
			if(os == OSX){
				if(down){
                    register_code16( KC_MISSION_CONTROL );
				//register_code16(KC_F3);
				}else{
                    unregister_code16( KC_MISSION_CONTROL );
				//unregister_code16(KC_F3);
				}
			}else{
				if(down){
				tap_code16(LWIN(KC_TAB));
				}
			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case SCR_OFF:
			if(down){
				interrupt_mods();
			}else{
				if(os == OSX){
				tap_code16(C(S(KC_KB_POWER)));
				}else{
				tap_code16(KC_SLEP);
				}
			}
			break;

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
			break;

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
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·
		/*
		//Opens the boot options menu. Changed to a Tap Dance: turn off / reboot
		case BOOT:
			if(down){
				interrupt_mods();
				if(os == OSX){
				tap_code16( C(KC_PWR) ); // sospecho que KC_PWR ahora es KC_KB_POWER
				}else{
				tap_code16( LWIN(KC_X) );
				tap_code16( KC_G );
				}
			}
			break;
		*/

		// · · · · · · · · · · · · · · · · · · · · · · · · ·

		case H_LOCK:
			if(down){
				interrupt_mods();
				toggle_hyper_lock();
			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·
        #ifdef RGB_MATRIX_ENABLE
		case RGBT_PREV:
		case RGBT_NEXT:
			if(down){
				interrupt_mods();
				rgb_rotate_theme(keycode == RGBT_NEXT ? 1 : -1);
			}
			break;
		case RGBR_PREV:
		case RGBR_NEXT:
			if(down){
				interrupt_mods();
				rgb_rotate_reactive(keycode == RGBR_NEXT ? 1 : -1);
			}
			break;
		case RGBA_PREV:
		case RGBA_NEXT:
			if(down){
				interrupt_mods();
				rgb_rotate_animation(keycode == RGBA_NEXT ? 1 : -1);
			}
			break;
		case RGB_UP:
		case RGB_DOWN:
			if(down){
				interrupt_mods();
				rgb_brightness(keycode == RGB_UP ? 1 : -1);
			}
			break;
        #endif
		// · · · · · · · · · · · · · · · · · · · · · · · · ·
		case NLOCK:
			if(down){
				interrupt_mods();
				num_lock = !num_lock;
				layer_move( num_lock ? _NL : _BASE );
				#ifdef RGB_MATRIX_ENABLE
					rgb_set_indicator(RGB_NUM_LOCK, num_lock);
				#endif
			}
			break;

		// · · · · · · · · · · · · · · · · · · · · · · · · ·
		case TERMINAL:
			if(down){
				interrupt_mods();
				tap_code16( XENO(KC_M) );
			}
			break;

		default:
			*handled = false;
			break;
	}

	if(*handled) *stop_propagation = true;
}
