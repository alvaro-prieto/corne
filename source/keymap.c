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

    Hysp keymap v0.80, (created by Alvaro Prieto Lauroba)
    =====================================================

    [ Warning ]
     ¯¯¯¯¯¯¯¯¯
    - You will not be able to compile this source directly. It contains some QMK core tweaks
      documented in notes.txt. Base QMK version was released at december 2, 2020. Tweaks were
      applied in order to fix some tap dance issues and provide compatibility to Apple keyboards


    [ Features ]
     ¯¯¯¯¯¯¯¯¯¯
    - Target user: spanish software developer using a macOS computer with a Windows bootcamp partition
    - Shotcuts are OS independent, which means they are translated into different key combinations
      according to the selected OS mode (macOs by default).
    - Custom per-key RGB themes and notificatns. Oline theme editor: https://codepen.io/alvaro-prieto/full/gOLrwKm
    - Modifier keys can behave as modifiers (if they are operated in conjunction with other keys),
      or as normal keys (if they are pressed independently).
    - Support for Os dependent key outputs, key sequences, unicode, and more.
    - Some keys has been moved from their usual location to improve accessibility and ergonomics
    - Hyper key. This key has been created to allow custom shortcuts depending on the focused application.
      For example, Hyper + I could mean "italic" in Word while being, "indent" in a code editor, and "invert" in Photoshop.
      You can implement this kind of behaviour in programs such as Karabiner-Elements.
    - Advanced Tap Dance: some keys behave differently, based on the amount of times that they have been tapped. Added
      compatibility for OS dependent key combinations, sequences, instant TD, extended uint16_t keycodes, and more.
    - Official Tap Dance source code has been fixed to allow multiple tap dances in a row without interruption issues.
    - A visual keymap APP has been created as reminder, using meaningful symbols and colors (not included here)


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
    - Check notes.txt in order to apply some minnor modifications to your QMK if you want to compile this source.
    - QMK version was released at december 2, 2020. This keymap will probably be ported to a newer QMK someday.
    - Check config.h for required keyboard ID specification to mimic a real Apple keyboard
    - Required APPLE_FN_ENABLE by Fauxpark, read notes.txt
    - Some modifications where made to Tap Dance source code and and rgb matrix state. Read notes.txt for further info
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
    F16 ln1, F17 ln2, F18 borde, F19 monitor, F20 flash close


   [ Dev tips ]
    ¯¯¯¯¯¯¯¯¯¯
    - If you are modifying this keymap and your keyboard doesn't output anything, probably you are running out
      of 'RAM' (Data Space). If so, try to move your data to Program Space using PROGMEM, or just add less stuff

    - During develpment, it is recomended to turn off: RGB_MATRIX_ENABLE=no  RGB_MATRIX_CUSTOM_USER=no in rules.mk
      to save some memory, and disable NO_DEBUG in config.h to be able to trace the code.

*/



const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


  //BLACK (default layer)
  [_BASE] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      ALT,     ES_Q,     ES_V,     TD(AC_E), ES_R,     ES_F,                            ES_Y,      TD(AC_U), TD(AC_I), TD(AC_O), ES_P,     ES_K,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CMD,     TD(AC_A), ES_S,     ES_D,     ES_T,     TD(G_DI),                        ES_H,      TD(ENYE), ES_B,     ES_L,     DEL,      ES_PLUS, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CTR,     ES_Z,     ES_X,     ES_W,     ES_C,     ES_MINS,                         ES_J,      ES_M,     ES_COMM,  DOT,      M5,       M6,      \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_SPC,   M2,            S1,     M3,        M4       ),
                                        //`-------------------------------´   `----------------------------'

  //PINK
  [_M1] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      KC_ESC,  TD(RSET), XXXXXXX,  MAXMZ,    REFRESH,  TD(DSK1),                         APPLE_F10, APPLE_F1, APPLE_F2, APPLE_F3, XXXXXXX,  XXXXXXX,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      SWAP_OS, XXXXXXX,  WIN_L,    RESTORE,  WIN_R,    TD(DSK2),                         APPLE_F11, APPLE_F4, APPLE_F5, APPLE_F6, XXXXXXX,  XXXXXXX,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      MUTE,    VOL_DOWN, VOL_UP,   MINMZ,    TD(EXIT), TD(DSK3),                         APPLE_F12, APPLE_F7, APPLE_F8, APPLE_F9, XXXXXXX,  XXXXXXX,\
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       XXXXXXX,  M2,            S1,      M3,        M4      ),
                                        //`-------------------------------´   `----------------------------'


  //PURPLE
  [_M2] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  SM_APP,   PREV_APP, NEXT_APP, EXPS,                            ONEMINS,  ES_1,      ES_2,     ES_3,     ES_PERC,  POW,     \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     KC_LWIN,  SELECT,   SAVE,     UNDO,     REDO,     SHW_DSK,                         TD(DECI), ES_4,      ES_5,     ES_6,     KC_BSPC,  TD(PRN), \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     SCR_OFF,  BR_DOWN,  BR_UP,    TD(PST),  TD(CP),   PASTE,                           TD(MUL),  ES_7,      ES_8,     ES_9,     INFNT,    XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       XXXXXXX,  M2,           S1,      ES_0,     TD(ADD)  ),
                                        //`-------------------------------´   `----------------------------'


  //RED
  [_M3] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
    ES_IEXL,   ES_IQUE,  TD(MRK),  KC_UP,    ES_LBRC,  ES_RBRC,                         TD(GRTR),  TD(TAGO), TD(TAGC), ES_PIPE,  ES_QUES,  ES_EXLM,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
    TD(STR12), ES_AT,    KC_LEFT,  KC_DOWN,  KC_RIGHT, FIELD,                           TD(LESS),  ES_LPRN, ES_RPRN,  ES_AMPR,  DEL_W,    ES_ASTR,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
    XXXXXXX,   XXXXXXX,  XXXXXXX,  ES_BSLS,  ES_HASH,  MEMBER,                          NOT_EQ,    ES_LCBR ,  ES_RCBR,  BULLET,   ES_SLSH,  ES_PERC,  \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,            S1,     M3,        M4       ),
                                        //`-------------------------------´   `----------------------------'


  //GREEN
  [_M4] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  KC_PGUP,  Z_OUT,    Z_IN,                            XXXXXXX,   RPLC,     EMOJI,    XXXXXXX,  TD(RSET), ELLIPSIS,\
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  TD(ORD),  W_LEFT,   KC_PGDN,  W_RIGHT,  XXXXXXX,                         SRCH_PR,   SEARCH,   SRCH_NX,  KC_UP,    KC_DEL,   XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  BORDER,   TD(LN),   TD(CMNT), LONGLN,                          XXXXXXX,   SPOTL,    KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_LSFT, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,             S1,    M3,        M4       ),
                                        //`-------------------------------´   `----------------------------'


  //BLUE
  [_M5] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  TOP,      XXXXXXX,  XXXXXXX,                         TD(GRTP),  XXXXXXX,  ES_NOT,   DEGR,     PI,       XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  FIRST,    BOTTOM,   LAST,     XXXXXXX,                         TD(LSTP),  WAVE,     XXXXXXX,  POUND,    TD(DLT),  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  MIDLN,                           TD(NTEQ),  XXXXXXX,  XXXXXXX,  ELLIPSIS, XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_LSFT,  M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´  `----------------------------'


  //BROWN
  [_M6] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  EURO,     REG,      XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  TD(SCR),  BOOT,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  TM,       XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  OSLOCK,   FLASH,    KBLOCK,  \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  CR,       TOPLN,                           XXXXXXX,   TD(APPS), SNG_PR,   SNG_NX,   PLAY,     XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             M1,       KC_SPC,   M2,            S1,     M3,        M4      ),
                                        //`-------------------------------´  `----------------------------'


  //HYPER (no color)
  [_HYP] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      ES_0,    ES_Q,      ES_V,    ES_E,     ES_R,     ES_F,                            ES_Y,      ES_U,     ES_I,     ES_O,     ES_P,     ES_K,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ES_1,    ES_A,      ES_S,    ES_D,     ES_T,     ES_G,                            ES_H,      ES_N,     ES_B,     ES_L,     KC_BSPC,  ES_PLUS, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      ES_2,    ES_Z,      ES_X,    ES_W,     ES_C,     ES_MINS,                         ES_J,      ES_M,     ES_COMM,  ES_DOT,   ES_9,     H_LOCK,  \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             ES_3,     ES_4,    ES_5,          ES_6,    ES_7,     ES_8    ),
                                        //`-------------------------------´   `----------------------------'


  //AUX (no color)
  [_MOD] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
      KC_LALT, ES_Q,     ES_V,     ES_D,     ES_R,     ES_F,                            ES_Y,      ES_U,     ES_I,     ES_O,     ES_P,     ES_K,    \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CMD,     ES_A,     ES_S,     ES_E,     ES_T,     ES_G,                            ES_H,      ES_N,     ES_B,     ES_L,     KC_BSPC,  ES_PLUS, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
      CTR,     ES_Z,     ES_X,     ES_W,     ES_C,     ES_MINS,                         ES_J,      ES_M,     ES_COMM,  ES_DOT,   XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             KC_LSFT,  KC_SPC,   KC_LSFT,       KC_ENT,  KC_LSFT,  KC_LSFT  ),
                                        //`-------------------------------´   `----------------------------'


  //ORANGE (cfg)
  [_CFG] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     XXXXXXX,  XXXXXXX,  XXXXXXX,  RGB_PREV, RGB_NEXT, XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  SWAP_OS,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  LNG,      XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,                         XXXXXXX,   XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             TO(_BASE),TO(_BASE),TO(_BASE),   TO(_BASE),TO(_BASE),TO(_BASE)),
                                        //`-------------------------------´  `----------------------------'

/*

  [_YY] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------------.                      ,-----------------------------------------------------------.
     _______,  _______,  _______,  _______,  _______,  _______,                         _______,   _______,  _______,  _______,  _______,  _______, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     _______,  _______,  _______,  _______,  _______,  _______,                         _______,   _______,  _______,  _______,  _______,  _______, \
  //|---------+---------+---------+---------+---------+---------|                      |----------+---------+---------+---------+---------+---------|
     _______,  _______,  _______,  _______,  _______,  _______,                         _______,   _______,  _______,  _______,  _______,  _______, \
  //|---------+---------+---------+---------+---------+---------+---------|  |---------+----------+---------+---------+---------+---------+---------|
                                             _______,  _______,  _______,      _______, _______,   _______ ),
                                        //`-------------------------------´  `----------------------------'



  [_XX] = LAYOUT_split_3x6_3( \
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




