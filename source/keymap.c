
#include QMK_KEYBOARD_H
#include <keymap_spanish.h>
#include "keydefs.c"
#include "handlers.c"
#include "dances.c"
#include "rgb.c"


/*
     _____                      
    / ____|                     
   | |     ___  _ __ _ __   ___ 
   | |    / _ \| '__| '_ \ / _ \
   | |___| (_) | |  | | | |  __/
    \_____\___/|_|  |_| |_|\___|
    
    Hysp keymap v0.77, (created by Alvaro Prieto Lauroba)
    =====================================================

    [ Features ]
     ¯¯¯¯¯¯¯¯¯¯

    - Target user: spanish software developer using a macOS computer with a Windows bootcamp partition
    - Shotcuts are OS independent, which means they are translated into different key combinations 
      according to the selected OS mode (macOs by default).
    - Modifier keys can behave as modifiers (if they are operated in conjunction with other keys), 
      or as normal keys (if they are pressed independently).
    - Support for Os dependent key outputs, key sequences, and more.
    - Some keys has been moved from their usual location to improve accesibility (C, V, B, W, Ñ, delete, ...)   
    - Hyper key. This key has been created to allow custom shortcuts depending on the focused application. 
      For example, Hyper + I could mean "italic" in Word while being, "indent" in a code editor, and "invert" in Photoshop.
      You can implement this kind of behaviour in programs such as Karabiner-Elements.
    - Advanced Tap Dance: specify keys behave differently, based on the amount of times they have been tapped. Added compatibility
      for OS dependent key combinations, sequences, and extended uint16_t keycodes.
    - A visual keymap has been created as reminder, using meaningful symbols and colors.     


    [ Hardware ]
     ¯¯¯¯¯¯¯¯¯¯
    - Developed for Corne LP with all extras included (RGB, OLED, etc). However
    - OLED is not being used since I didn't find it useful and I preffer to save some bytes


    [ Passwords ]
     ¯¯¯¯¯¯¯¯¯¯
    - To unlok the keyboard type "yo". It will flash a purple notification to let you know that it is active.
      If you want to change the keyword, you can change it in before_key_handler to something like "me" or "hi"

    [ Settings ]
     ¯¯¯¯¯¯¯¯¯¯

    - Check config.h for required keyboard ID specification to mimic a real Apple keyboard
    - Required APPLE_FN_ENABLE, use this patch: https://gist.github.com/fauxpark/010dcf5d6377c3a71ac98ce37414c6c4
      (download the raw file, then run git apply <path-to-patch-file> in your qmk_firmware dir)
    - It is required a callback to detect when RGB is suspended, to do so, add this to rgb_matrix.c:
      __attribute__((weak)) void rgb_matrix_suspend_state_changed( bool suspend_state) {}
      and also, in the correct place inside of rgb_matrix_task:
      if(suspend_backlight != last_suspend_state){
        last_suspend_state = suspend_backlight;
        rgb_matrix_suspend_state_changed( suspend_backlight );
      }
    - Some shortcuts have not an equivalence in different OS.
    - In macOS settings -> keyboard. Disable: "Use F1, F2 , etc. keys as standard function keys"
    - In Windows, advanced energy settings: Sleep : turn off screen.
    - In macOS, split screen features achieved using BetterSnapTool
    - If you are facing problems in characters such as: ª, >, and others not outputing the expected value, 
      it might depend in your OS keyboard regional settings. Go to Karabiner and select, Country code: 1
      for your Corne keyboard.
    - Turning off your computer shortcut in Windows might change according to your OS language.
    - Some shortcuts / outputs, might require third party software.
    - Desktop features in macOS require 3 desktops and Change to desktop shortcuts enabled in 
      Keyboard -> shortcuts -> mission control. 


    [ External software dependent features ]
     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
     
    - Per-app hyper shortcuts
    - Split screen features in macOs
    - Lines (key output)
    - Moving APP to certain desktop


    [ Used function keys FN ]
     ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
    16 ln1, 17, ln2, 18 borde, F19 monitor

*/



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  
  [_BASE] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      CMD,     ES_Q,     ES_V,     TD(AC_E), ES_R,      ES_T,                           ES_Y,     ES_U,     ES_I,      ES_O,     KC_P,     ES_K,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ALT,     TD(AC_A), ES_S,     ES_D,     ES_C,      ES_F,                           ES_H,     ES_N,     ES_ACUT,   ES_L,     KC_BSPC,  ES_NTIL, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CTR ,    ES_Z,     ES_X,     ES_W,     ES_B,      ES_G,                           ES_J,     ES_M,     ES_COMM,   ES_DOT,   M5,       M6,      \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,        KC_SPC,  M2,            S1,     M3,       M4        ),
                                        //`-------------------------------´   `----------------------------'
  
  [_M1] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      KC_ESC,  TD(RSET), SM_APP,   PREV_APP, NEXT_APP, REFRESH,                         APPLE_F11, APPLE_F6, APPLE_F7, APPLE_F8, APPLE_F9, APPLE_F12,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      SWAP_OS, SELECT,   SAVE,     UNDO,    REDO,      PST_CLEAN,                       APPLE_F10, APPLE_F1, APPLE_F2, APPLE_F3, APPLE_F4, APPLE_F5,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      KC_LWIN, UNDO,     CUT,      COPY,    PASTE,     PST_PLACE,                       XXXXXXX,   XXXXXXX,  XXXXXXX,  S(ES_3),  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,  XXXXXXX,  M2,                 S1,     M3,        M4      ),
                                        //`-------------------------------´   `----------------------------'

  [_M2] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     MONITOR,  CLS_APPS, XXXXXXX,  MAXMZ,   EXPS,     TD(DSK1),                         ONEPLUS,  ES_6,      ES_7,     ES_8,     ES_9,     INFNT,   \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     BR_DOWN,  BR_UP,    WIN_L,  RESTORE,   WIN_R,    TD(DSK2),                         ES_0,     ES_1,      ES_2,     ES_3,     ES_4,     ES_5,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     VOL_DOWN, VOL_UP,   TD(EXIT), MINMZ,    SHW_DSK, TD(DSK3),                         KC_SPC,   ONEMINS,  ES_COMM,   ES_DOT,   ES_ASTR,  ES_SLSH, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       XXXXXXX,  M2,            S1,     ES_PLUS,  ES_MINS  ),
                                        //`-------------------------------´   `----------------------------'      
   
  [_M3] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  ES_QUES,  ES_AT,    KC_UP,    ES_PLUS,  ES_MINS,                         TD(TAG_C), ES_LCBR,  ES_RCBR,  ES_PIPE,  TD(SCR), TD(STR12),\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     ES_HASH,  ES_EXLM,  KC_LEFT,  KC_DOWN,  KC_RIGHT, TD(ARROWS),                      TD(TAG_O), ES_LPRN,  ES_RPRN,  ES_AMPR,  DEL_W,    WAVE,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  ES_UNDS,  ES_BSLS,  ES_PERC,  ES_ASTR,  ES_SLSH,                         NOT_EQ,    ES_LBRC,  ES_RBRC,  ELLIPSIS, ES_GRV,   XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,            S1,     M3,        M4       ),
                                        //`-------------------------------´   `----------------------------' 
        
  [_M4] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  QUESTN,   TD(ORD),  KC_PGUP,  Z_OUT,    Z_IN,                            TD(GRTR),  SRCH_PR,  SRCH_NX,  EMOJI,    TD(RSET), KC_ESC,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  EXCLAM,   W_LEFT,   KC_PGDN,  W_RIGHT,  XXXXXXX,                         TD(LESS),  SEARCH,  XXXXXXX,   KC_UP,    KC_DEL,   SWAP_OS, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  MIDLN,    XXXXXXX,  XXXXXXX, TD(COMMNT),TD(LINES),                       XXXXXXX,   SPOTL,   KC_LEFT,   KC_DOWN,  KC_RIGHT, KC_LSFT, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,             S1,    M3,        M4       ),
                                        //`-------------------------------´   `----------------------------' 
  
  [_M5] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  TOP,      XXXXXXX,  XXXXXXX,                         TD(GRTP),  XXXXXXX,  XXXXXXX,  DEGR,     PI,       ES_CIRC, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  FIRST,    BOTTOM,   LAST,     XXXXXXX,                         TD(LSTP),  XXXXXXX,  XXXXXXX,  POUND,    DEL_LN,   ES_NOT,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  LONGLN,   XXXXXXX,  XXXXXXX,  XXXXXXX,  BORDER,                          TD(NTEQ),  XXXXXXX,  XXXXXXX,  BULLET,   XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´  `----------------------------' 

   [_M6] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  EURO,     REG,      TM,                              XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  LOCK,     BOOT,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  CR,       XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  SCR_OFF, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  TOPLN,    XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  SNG_PR,   SNG_NX,   PLAY,     XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_SPC,   M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´  `----------------------------' 
      
  [_HYP] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      ES_0,    ES_Q,      ES_V,    ES_E,     ES_R,     ES_T,                             ES_Y,     ES_U,     ES_I,     ES_O,    ES_P,      ES_K,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ES_1,    ES_A,      ES_S,    ES_D,     ES_C,     ES_F,                             ES_H,     ES_N,     ES_ACUT,  ES_L,    S(KC_DEL), ES_NTIL, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ES_2,    ES_Z,      ES_X,    ES_W,     ES_B,     ES_G,                             ES_J,     ES_M,     ES_COMM,  ES_DOT,  ES_9,      H_LOCK,  \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             ES_3,     ES_4,    ES_5,          ES_6,     ES_7,     ES_8    ),
                                        //`-------------------------------´   `----------------------------'   
   
  [_MOD] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      CMD,     ES_Q,      ES_V,    ES_E,     ES_R,      ES_T,                            ES_Y,     ES_U,     ES_I,      ES_O,    ES_P,     ES_K,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      KC_LALT, ES_A,      ES_S,    ES_D,     ES_C,      ES_F,                            ES_H,     ES_N,     ES_ACUT,   ES_L,    KC_BSPC,  ES_NTIL, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CTR,     ES_Z,      ES_X,    ES_W,     ES_B,      ES_G,                            ES_J,     ES_M,     ES_COMM,   ES_DOT,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             KC_LSFT,   KC_SPC,  KC_LSFT,       KC_ENT,  KC_LSFT,  KC_LSFT  ),
                                        //`-------------------------------´   `----------------------------'    

   [_EXT] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  RGB_PREV, RGB_NEXT, XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  KBLOCK,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             TO(_BASE),TO(_BASE),TO(_BASE),   TO(_BASE),TO(_BASE),TO(_BASE)),
                                        //`-------------------------------´  `----------------------------'  
/*
  
  [_S2] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     _______,  _______,  _______,  _______,  _______,  _______,                         _______,   _______,  _______,  _______,  _______,  _______, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     _______,  _______,  _______,  _______,  _______,  _______,                         _______,   _______,  _______,  _______,  _______,  _______, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     _______,  _______,  _______,  _______,  _______,  _______,                         _______,   _______,  _______,  _______,  _______,  _______, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             _______,  _______,  _______,      _______, _______,   _______ ),
                                        //`-------------------------------´  `----------------------------'  


  [_S2] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             XXXXXXX,  XXXXXXX,  XXXXXXX,      XXXXXXX, XXXXXXX,   XXXXXXX ),
                                        //`-------------------------------´  `----------------------------'  
*/
        
};




