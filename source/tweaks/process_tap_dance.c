/* Copyright 2016 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "quantum.h"
#include "action_tapping.h"


#ifndef NO_ACTION_ONESHOT
uint8_t get_oneshot_mods(void);
#endif

static uint16_t last_td;
static int8_t   highest_td = -1;
static uint16_t interruption_first_td = 0;
static uint16_t interruption_second_td = 0;
static uint16_t last_td_release = 0;

void qk_tap_dance_pair_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
    qk_tap_dance_pair_t *pair = (qk_tap_dance_pair_t *)user_data;
    if (state->count == 2) {
        register_code16(pair->kc2);
        state->finished = true;
    }
}

void qk_tap_dance_pair_finished(qk_tap_dance_state_t *state, void *user_data) {
    qk_tap_dance_pair_t *pair = (qk_tap_dance_pair_t *)user_data;
    if (state->count == 1) {
        register_code16(pair->kc1);
    } else if (state->count == 2) {
        register_code16(pair->kc2);
    }
}

void qk_tap_dance_pair_reset(qk_tap_dance_state_t *state, void *user_data) {
    qk_tap_dance_pair_t *pair = (qk_tap_dance_pair_t *)user_data;
    if (state->count == 1) {
        unregister_code16(pair->kc1);
    } else if (state->count == 2) {
        unregister_code16(pair->kc2);
    }
}

void qk_tap_dance_dual_role_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
    qk_tap_dance_dual_role_t *pair = (qk_tap_dance_dual_role_t *)user_data;
    if (state->count == 2) {
        layer_move(pair->layer);
        state->finished = true;
    }
}

void qk_tap_dance_dual_role_finished(qk_tap_dance_state_t *state, void *user_data) {
    qk_tap_dance_dual_role_t *pair = (qk_tap_dance_dual_role_t *)user_data;
    if (state->count == 1) {
        register_code16(pair->kc);
    } else if (state->count == 2) {
        pair->layer_function(pair->layer);
    }
}

void qk_tap_dance_dual_role_reset(qk_tap_dance_state_t *state, void *user_data) {
    qk_tap_dance_dual_role_t *pair = (qk_tap_dance_dual_role_t *)user_data;
    if (state->count == 1) {
        unregister_code16(pair->kc);
    }
}

static inline void _process_tap_dance_action_fn(qk_tap_dance_state_t *state, void *user_data, qk_tap_dance_user_fn_t fn) {
    if (fn) {
        //uprintf("fn\n");
        fn(state, user_data);
    }
}

static inline void process_tap_dance_action_on_each_tap(qk_tap_dance_action_t *action) {
    //uprintf("action each\n");
    _process_tap_dance_action_fn(&action->state, action->user_data, action->fn.on_each_tap);
}

static inline void process_tap_dance_action_on_dance_finished(qk_tap_dance_action_t *action) {
    //uprintf("action finish\n");
    if (action->state.finished) return;
    action->state.finished = true;
    add_mods(action->state.oneshot_mods);
    add_weak_mods(action->state.weak_mods);
    send_keyboard_report();
    _process_tap_dance_action_fn(&action->state, action->user_data, action->fn.on_dance_finished);
}

static inline void process_tap_dance_action_on_reset(qk_tap_dance_action_t *action) {
    //uprintf("no? action reset\n");
    _process_tap_dance_action_fn(&action->state, action->user_data, action->fn.on_reset);
    del_mods(action->state.oneshot_mods);
    del_weak_mods(action->state.weak_mods);
    send_keyboard_report();
}

/*
elefante (keyword para búsquedas)
DEFINICIONES:

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

typedef struct {
    uint8_t  count;
    uint8_t  oneshot_mods;
    uint8_t  weak_mods;
    uint16_t keycode;
    uint16_t interrupting_keycode;
    uint16_t timer;
    bool     interrupted;
    bool     pressed;
    bool     finished;
} qk_tap_dance_state_t;


typedef struct {
    struct {
        qk_tap_dance_user_fn_t on_each_tap;
        qk_tap_dance_user_fn_t on_dance_finished;
        qk_tap_dance_user_fn_t on_reset;
    } fn;
    qk_tap_dance_state_t state;
    uint16_t             custom_tapping_term;
    void *               user_data;
} qk_tap_dance_action_t;
*/

void preprocess_tap_dance(uint16_t keycode, keyrecord_t *record) {
    qk_tap_dance_action_t *action;
    //uprintf("preprocess\n");
    if (!record->event.pressed){
        //uprintf("prerelease (press, keycode, interrupted, pressed, finished): %d  %d  %d  %d  %d \n", record->event.pressed, keycode, action->state.interrupted, action->state.pressed, action->state.finished);
        return;
    }

    if (highest_td == -1) return;
   // uprintf("Inicia bucle------------------------------\n ");
    for (int i = 0; i <= highest_td; i++) {
        action = &tap_dance_actions[i];
        if (action->state.count) {
            if (keycode == action->state.keycode && keycode == last_td) continue;
            //uprintf("press (press, keycode, interrupted, pressed, finished, index): %d  %d  %d  %d  %d  %d \n", record->event.pressed, keycode, action->state.interrupted, action->state.pressed, action->state.finished, i);
            //uprintf("(keycode, 1st, 2nd, last_td, last_td_release) %d %d %d %d %d\n", keycode, interruption_first_td, interruption_second_td, last_td, last_td_release);
            if(keycode == interruption_second_td && last_td_release == interruption_first_td){
                //uprintf("PREVENIDO\n");
                interruption_first_td = interruption_second_td = 0;
            }else{
                action->state.interrupted          = true;
                action->state.interrupting_keycode = keycode;
                process_tap_dance_action_on_dance_finished(action);
                reset_tap_dance(&action->state);
            }

            //Cuando un TD es interrumpido, y la tecla que interrumpe es otro TD
            //almacenamos los TD implicados para cuando se produzca el release del primer TD
            //evitando que interrumpa al segundo
            if(action->state.pressed && keycode >= QK_TAP_DANCE && keycode <= QK_TAP_DANCE_MAX){
                interruption_first_td = action->state.keycode;
                interruption_second_td = keycode;
                //uprintf("WARN 1st 2nd: %d %d \n", interruption_first_td, interruption_second_td);
            }


        }
    }
    //uprintf("========================== Bucle fin \n ");
}

bool process_tap_dance(uint16_t keycode, keyrecord_t *record) {
    uint16_t               idx = keycode - QK_TAP_DANCE;
    qk_tap_dance_action_t *action;
    //uprintf("process\n");
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            if ((int16_t)idx > highest_td) highest_td = idx;
            action = &tap_dance_actions[idx];

            action->state.pressed = record->event.pressed;
            if (record->event.pressed) {
                //uprintf("process down\n");
                action->state.keycode = keycode;
                action->state.count++;
                action->state.timer = timer_read();
#ifndef NO_ACTION_ONESHOT
                action->state.oneshot_mods = get_oneshot_mods();
#else
                action->state.oneshot_mods = 0;
#endif
                action->state.weak_mods = get_mods();
                action->state.weak_mods |= get_weak_mods();
                process_tap_dance_action_on_each_tap(action);

                last_td = keycode;
            } else {
                //uprintf("process up\n");
                //uprintf("interrupted? intr_key? %d %d \n", action->state.interrupted, action->state.interrupting_keycode);
                if(action->state.interrupted){
                    last_td_release = keycode;
                    //uprintf("last_td_release %d \n", last_td_release);
                }
                if (action->state.count && action->state.finished) {
                    //uprintf("process reset\n");
                    reset_tap_dance(&action->state);
                }

            }

            break;
    }

    return true;
}

void matrix_scan_tap_dance() {
    if (highest_td == -1) return;
    uint16_t tap_user_defined;

    for (uint8_t i = 0; i <= highest_td; i++) {
        qk_tap_dance_action_t *action = &tap_dance_actions[i];
        if (action->custom_tapping_term > 0) {
            tap_user_defined = action->custom_tapping_term;
        } else {
            tap_user_defined = get_tapping_term(action->state.keycode, NULL);
        }
        if (action->state.count && timer_elapsed(action->state.timer) > tap_user_defined) {
            //uprintf("scan %d \n", action->state.keycode);
            process_tap_dance_action_on_dance_finished(action);
            reset_tap_dance(&action->state);
        }
    }
}

void reset_tap_dance(qk_tap_dance_state_t *state) {
    qk_tap_dance_action_t *action;
    //uprintf("reseteo interno\n");

    if (state->pressed){
        //uprintf("reseteo prevenido\n");
        return;
    }

    action = &tap_dance_actions[state->keycode - QK_TAP_DANCE];

    process_tap_dance_action_on_reset(action);

    state->count                = 0;
    state->interrupted          = false;
    state->finished             = false;
    state->interrupting_keycode = 0;
    last_td                     = 0;
}
