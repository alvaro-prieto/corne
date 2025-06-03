#include "shared.h"
#include "sync.h"
#include "handlers.h"
#ifdef RGB_MATRIX_ENABLE
#include "rgb.h"
#endif

//═══════════════════════════════════════════════════════════════
//   MAIN  (entry point)
//═══════════════════════════════════════════════════════════════


//  KB LIFECYCLE
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––

void keyboard_pre_init_user(void) {}

void matrix_init_user(void) {}

void keyboard_post_init_user(void) {
	master = is_keyboard_master();
	init_sync();
	#ifdef RGB_MATRIX_ENABLE
	post_init_rgb();
	#endif
}

void suspend_power_down_user(void) {
	xprintf("powerdown\n");
}

void suspend_wakeup_init_user(void) {
	xprintf("wakeup\n");
}

//  MAIN LOOP
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool down = record->event.pressed;
    return handle_key_event(keycode, down, record);
}

void housekeeping_task_user(void) {
    if( master && kb_is_synced() ){
        process_sync_queue();
    }
}



