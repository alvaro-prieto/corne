
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
    
    Hysp keymap v0.79, (created by Alvaro Prieto Lauroba)
    =====================================================


    [ Features ]
     ¯¯¯¯¯¯¯¯¯¯

    - Target user: spanish software developer using a macOS computer with a Windows bootcamp partition
    - Shotcuts are OS independent, which means they are translated into different key combinations 
      according to the selected OS mode (macOs by default).
    - Modifier keys can behave as modifiers (if they are operated in conjunction with other keys), 
      or as normal keys (if they are pressed independently).
    - Support for Os dependent key outputs, key sequences, ASCII, and more.
    - Some keys has been moved from their usual location to improve accessibility and ergonomics   
    - Hyper key. This key has been created to allow custom shortcuts depending on the focused application. 
      For example, Hyper + I could mean "italic" in Word while being, "indent" in a code editor, and "invert" in Photoshop.
      You can implement this kind of behaviour in programs such as Karabiner-Elements.
    - Advanced Tap Dance: some keys behave differently, based on the amount of times that they have been tapped. Added
      compatibility for OS dependent key combinations, sequences, and extended uint16_t keycodes.
    - A visual keymap has been created as reminder, using meaningful symbols and colors.     


    [ Hardware ]
     ¯¯¯¯¯¯¯¯¯¯
    - Developed for Corne LP with all extras included (RGB, OLED, etc). However OLED 
      is not being used since I didn't find it useful and I preffer to save some bytes


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
    - In Windows Unicode is managed using WinCompose, in macOs using custom key combos (to prevent Unicode Hex)
    - If you are facing problems in characters such as: ª, >, and others not outputing the expected value, 
      it might depend in your OS keyboard regional settings. Go to Karabiner and select, Country code: 1
      for your Corne keyboard. MacOS settings is set to "Spanish - ISO"
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


   [ Dev tips ]
    ¯¯¯¯¯¯¯¯¯¯
    - If you are modifying this keymap and your keyboard doesn't output anything, probably you are running out
      of 'RAM' (Data Space). If so, try to move your data to Program Space using PROGMEM, or just add less stuff


*/



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  
  
  //BLACK (default layer)
  [_BASE] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      CMD,     ES_Q,     ES_V,     TD(AC_E), ES_R,     ES_F,                            ES_Y,      TD(AC_U), TD(AC_I), TD(AC_O), ES_P,     DOT,     \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ALT,     TD(AC_A), ES_S,     ES_D,     ES_T,     ES_G,                            ES_H,      TD(ENYE), ES_B,     ES_L,     DEL,      TD(MRK), \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CTR ,    ES_Z,     ES_X,     ES_W,     ES_C,     ES_MINS,                         ES_J,      ES_M,     ES_COMM,  ES_K,     M5,       M6,      \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_SPC,   M2,            S1,     M3,        M4        ),
                                        //`-------------------------------´   `----------------------------'
  
  
  //PINK
  [_M1] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      KC_ESC,  TD(RSET), XXXXXXX,  MAXMZ,    REFRESH,  XXXXXXX,                         APPLE_F11, APPLE_F6, APPLE_F7, APPLE_F8, APPLE_F9, APPLE_F12,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      SWAP_OS, XXXXXXX,  WIN_L,    RESTORE,  WIN_R,    XXXXXXX,                         APPLE_F10, APPLE_F1, APPLE_F2, APPLE_F3, APPLE_F4, APPLE_F5,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      BR_DOWN, BR_UP,    XXXXXXX,  MINMZ,    XXXXXXX,  XXXXXXX,                         XXXXXXX,   TD(DSK1),  TD(DSK2), TD(DSK3), XXXXXXX,  XXXXXXX,\
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       XXXXXXX,  M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´   `----------------------------'

  
  //PURPLE
  [_M2] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     KC_LWIN,  TD(APPS), SM_APP,   PREV_APP, NEXT_APP, EXPS,                            ONEPLUS,  ES_6,      ES_7,     ES_8,     ES_9,     INFNT,   \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  SELECT,   SAVE,     UNDO,     REDO,     SHW_DSK,                         ES_0,     ES_1,      ES_2,     ES_3,     ES_4,     ES_5,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     VOL_DOWN, VOL_UP,   TD(EXIT), TD(PST),  TD(CP),   PASTE,                           KC_SPC,   ONEMINS,   ES_COMM,  ES_DOT,   XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       XXXXXXX,  M2,            S1,     TD(ADD),  TD(MUL)  ),
                                        //`-------------------------------´   `----------------------------'      
  

  //RED
  [_M3] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  ES_HASH,  ES_DIAE,  KC_UP,    ES_ASTR,  ES_SLSH,                         TD(GRTR),  TD(CURL), TD(SQR),  ES_PIPE,  ES_QUES,  ES_EXLM, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  ES_AT,    KC_LEFT,  KC_DOWN,  KC_RIGHT, FIELD,                           TD(LESS),  TD(PRN),  ES_RPRN,  ES_AMPR,  DEL_W,    XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  ES_BSLS,  ES_PERC,  MEMBER,   ES_PLUS,                         NOT_EQ,    TD(TAGO), TD(TAGC), XXXXXXX,  ES_GRV,  TD(STR12),\
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,            S1,     M3,        M4       ),
                                        //`-------------------------------´   `----------------------------' 
  

  //GREEN
  [_M4] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PGUP,  Z_OUT,    Z_IN,                            EMOJI,     RPLC,     XXXXXXX,  POUND,    TD(RSET), ELLIPSIS,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  TD(ORD),  W_LEFT,   KC_PGDN,  W_RIGHT,  XXXXXXX,                         SRCH_PR,   SEARCH,   SRCH_NX,  KC_UP,    KC_DEL,   XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  TD(LN),   BORDER,  XXXXXXX,   TD(CMNT), LONGLN,                          XXXXXXX,   SPOTL,    KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_LSFT, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,             S1,    M3,        M4       ),
                                        //`-------------------------------´   `----------------------------' 
  
  
  //BLUE
  [_M5] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  TOP,      XXXXXXX,  XXXXXXX,                         TD(GRTP),  XXXXXXX,  ES_NOT,   DEGR,     PI,       BULLET,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  FIRST,    BOTTOM,   LAST,     XXXXXXX,                         TD(LSTP),  WAVE,     XXXXXXX,  POUND,    XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  MIDLN,                           TD(NTEQ),  XXXXXXX,  XXXXXXX,  ES_CIRC,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´  `----------------------------' 
  
 
  //BROWN
  [_M6] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  EURO,     REG,      XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  TD(SCR),  BOOT,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  TM,       XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  KBLOCK,   LOCK,     SCR_OFF, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  CR,       TOPLN,                           XXXXXXX,   XXXXXXX,  SNG_PR,   SNG_NX,   PLAY,     XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_SPC,   M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´  `----------------------------' 
      

  //AUX (no color)
  [_HYP] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      ES_0,    ES_Q,      ES_V,    ES_E,     ES_R,     ES_F,                            ES_Y,      ES_U,     ES_I,     ES_O,     ES_P,     ES_DOT,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ES_1,    ES_A,      ES_S,    ES_D,     ES_T,     ES_G,                            ES_H,      ES_N,     ES_B,     ES_L,     S(KC_DEL),XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ES_2,    ES_Z,      ES_X,    ES_W,     ES_C,     ES_MINS,                         ES_J,      ES_M,     ES_COMM,  ES_K,     ES_9,     H_LOCK,  \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             ES_3,     ES_4,    ES_5,          ES_6,    ES_7,     ES_8    ),
                                        //`-------------------------------´   `----------------------------'   
  

  //AUX (no color) 
  [_MOD] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      CMD,     ES_Q,     ES_V,     ES_D,     ES_R,     ES_F,                            ES_Y,      ES_U,     ES_I,     ES_O,     ES_P,     ES_DOT,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      KC_LALT, ES_A,     ES_S,     ES_E,     ES_T,     ES_G,                            ES_H,      ES_N,     ES_B,     ES_L,     KC_BSPC,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CTR,     ES_Z,     ES_X,     ES_W,     ES_C,     ES_MINS,                         ES_J,      ES_M,     ES_COMM,  ES_K,     XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             KC_LSFT,  KC_SPC,   KC_LSFT,       KC_ENT,  KC_LSFT,  KC_LSFT  ),
                                        //`-------------------------------´   `----------------------------'    

  
  //ORANGE
  [_EXT] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  RGB_PREV, RGB_NEXT, XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
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




