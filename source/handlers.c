
#include "shared.h"
#include "sync.h"
#include "handlers.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═════════════════════════════════════════════════════════
//    KEY EVENT HANDLERS  (entry point)
//═════════════════════════════════════════════════════════

// Entry point (-> process_record_user)
// Handle key events according to its type
//––––––––––––––––––––––––––––––––––––––––––––––––––
bool handle_key_event(uint16_t keycode, bool down, keyrecord_t *record) {
	bool stop_propagation = false;
	bool handled = false;
    if( master && !kb_lock ) keypress_event( keycode, down, record );
	KeyHandler handler_function = NULL;
	switch (keycode) {

        // testing
		case TEST ... TEST2:
			handler_function = handle_test_key;
			break;

        // modifiers
		case FIRST_MODIFIER_INDEX ... LAST_MODIFIER_INDEX:
			handler_function = handle_modifier;
			break;

        // sequence keys
		case FIRST_SEQUENCE_INDEX ... LAST_SEQUENCE_INDEX:
			handler_function = handle_sequence;
			break;

        // OS dependent keys
		case FIRST_OS_INDEX ... LAST_OS_INDEX:
			handler_function = handle_os_key;
			break;

        // basic keys, no mods
		case KC_END: case KC_HOME: case KC_PGDN: case KC_PGUP:
		case ES_1 ... ES_0:
			handler_function = handle_unmoded;
			break;

        // unicode keys
		case FIRST_UNICODE_INDEX ... LAST_UNICODE_INDEX:
			handler_function = handle_unicode;
			break;
        // alias keys
		case FIRST_ALIAS_INDEX ... LAST_ALIAS_INDEX:
			handler_function = handle_alias;
			break;
	}

  	KeyHandler handlers[] = {
        before_handling,
        handle_accents,
        handle_caps_word,
		handler_function,
		handle_custom,
		handle_default
	};
    const int num_handlers = sizeof(handlers) / sizeof(handlers[0]);
    for (int i = 0; i < num_handlers; i++) {
        if( handlers[i] ) handlers[i]( keycode, down, record, &stop_propagation, &handled );
        if( handled ) return !stop_propagation;
    }
    return true;
}



// Mod handler
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_modifier(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	mod_key_handler(keycode, down, record); // y si lo estandarizo?
	mod_combo( down );
	*handled = true;
	*stop_propagation = true;
}


// Sequence handler
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_sequence(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	if(down){
		interrupt_mods();
		tap_sequence( keycode );
	}
	*handled = true;
	*stop_propagation = true;
}

// OS dependent key handler
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_os_key(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	if(down){
		interrupt_mods();
		register_os_dependent_key( keycode );
	}else{
		unregister_os_dependent_key( keycode );
	}
	*handled = true;
	*stop_propagation = true;
}

// Unicode key handler
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_unicode(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	if(down){
		interrupt_mods();
		tap_unicode_key( keycode );
	}
	*handled = true;
	*stop_propagation = true;
}

// Alias key handler
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_alias(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	if(down){
		interrupt_mods();
		tap_alias_key( keycode );
	}
	*handled = true;
	*stop_propagation = true;
}

// Unmoded key handler
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_unmoded(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
	if(down){
		interrupt_mods();
		register_code16( keycode );
	}else{
		unregister_code16( keycode );
	}
	*handled = true;
	*stop_propagation = true;
}

// Default handler (uhandled handler)
//––––––––––––––––––––––––––––––––––––––––––––––––––
void handle_default(uint16_t keycode, bool down, keyrecord_t *record, bool *stop_propagation, bool *handled){
    if(IS_LAYER_ON(_HYP)){
        // xprintf( "hyper\n" );
        if(down){
            register_code16(keycode);
        }else{
            unregister_code16(keycode);
        }

        *handled = true;
		*stop_propagation = true;
        return;
    }
	if( modsNumber > 0 && down && !IS_LAYER_ON(_HYP) ){
		interrupt_mods();
	}
	// The following lines could be skipped, but then, holding down combined keycodes
	// could contamine the following ones if not fully released. For example: %+3 = %·

	// Detect shifted keycodes:
	if((keycode >= QK_LSFT && keycode <= (QK_LSFT + 0xFF)) ||  (keycode >= QK_RSFT && keycode <= (QK_RSFT + 0xFF))){
		// avoid hold down shifted keycodes to prevent contamination for fast typist
		if(down){
			tap_code16( keycode );
		}
		*handled = true;
		*stop_propagation = true;
	}
}
