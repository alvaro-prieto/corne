#pragma once
#include "shared.h"

// Key event handler entry point (process_record_user)
bool handle_key_event(uint16_t keycode, bool down, keyrecord_t *record);

// Before handlers
HANDLE_KEY_EVENT( before_handling );

// Smart handlers
HANDLE_KEY_EVENT( handle_caps_word );
HANDLE_KEY_EVENT( handle_accents );

// Custom handlers
HANDLE_KEY_EVENT(handle_custom);

// Range handlers
HANDLE_KEY_EVENT(handle_test_key);
HANDLE_KEY_EVENT(handle_modifier);
HANDLE_KEY_EVENT(handle_function_key);
HANDLE_KEY_EVENT(handle_sequence);
HANDLE_KEY_EVENT(handle_os_key);
HANDLE_KEY_EVENT(handle_unicode);
HANDLE_KEY_EVENT(handle_alias);
HANDLE_KEY_EVENT(handle_unmoded);
HANDLE_KEY_EVENT(handle_default);

// Mods
void mod_combo( bool down );
bool mod_is_active(Keypress *kp);
bool mod_key_handler( uint16_t keycode, bool down, keyrecord_t *record );
bool hold_key_handler( uint16_t keycode, bool down, keyrecord_t *record );
void hyper( bool down );
void toggle_hyper_lock( void );
void passive_down( uint16_t key );
void passive_up( void );
void trigger_mod_combo( uint8_t mask, bool down);
void interrupt_mods( void );

// Custom hooks
void shiftUpHandler( Keypress *kp );
bool tildeDownHandler( Keypress *kp );
bool shiftDownHandler( Keypress *kp );
void shiftUpHandler( Keypress *kp );
bool M1DownHandler ( Keypress *kp );
void M1UpHandler ( Keypress *kp );
void M2UpHandler ( Keypress *kp );
bool M6DownHandler ( Keypress *kp );
bool cmdDownHandler( Keypress *kp );
bool ctrDownHandler( Keypress *kp );
void ctrUpHandler( Keypress *kp );
