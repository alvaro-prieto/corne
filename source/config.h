#pragma once

//Impersonate an Apple keyboard
#undef VENDOR_ID
#undef DEVICE_VER
#undef PRODUCT_ID
#undef MANUFACTURER
#undef MAINTAINER
#define VENDOR_ID       0x05ac
#define DEVICE_VER      0x0074
#define PRODUCT_ID      0x0250
#define MANUFACTURER    "Apple Inc."
#define TAP_HOLD_CAPS_DELAY 110

//keylog length (passwords and auto-diaeresis)
#define LOG_SIZE 4

//no need for underglow in a closed case.
#define DISABLE_UNDERGLOW
#define LOCK_KB_ON_LOCK_OS
#define TAPPING_TERM 250
#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE
//#define UNICODE_CYCLE_PERSIST false
#define OVERFLOW_SMALL 15
#define MASTER_LEFT
//#define SPLIT_LAYER_STATE_ENABLE
#define LAYER_STATE_16BIT

#define SPLIT_TRANSACTION_IDS_USER RPC_ID_RGB_SYNC

//#define PERMISSIVE_HOLD
#undef SSD1306OLED

#ifdef COMBO_ENABLE
    //#define TAPPING_TERM_PER_KEY
    //#define IGNORE_MOD_TAP_INTERRUPT
    //#define PERMISSIVE_HOLD_PER_KEY
    //#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
    #define COMBO_TERM 50
    #define EXTRA_SHORT_COMBOS
    #define COMBO_SHOULD_TRIGGER
#endif


//comunication protocol
//#define USE_I2C
//#undef USE_I2C
//#define USE_SERIAL_PD2 //obsoleto


//RGB backlight
#ifdef RGB_MATRIX_ENABLE

	#define RGB_DISABLE_WHEN_USB_SUSPENDED true
    #define RGB_MATRIX_TIMEOUT 20 * 1000    //suspend timeout
	#define RGB_MATRIX_LED_FLUSH_LIMIT 35  // milliseconds (high values increases keyboard responsiveness). 16 (16ms) is equivalent to limiting to 60fps
	#define RGB_MATRIX_LED_PROCESS_LIMIT DRIVER_LED_TOTAL
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120 //WARNING! limits maximum brightness of LEDs to 120 out of 255. Higher may cause the controller to crash.
	#define RGB_MATRIX_HUE_STEP 8
	#define RGB_MATRIX_SAT_STEP 8
	#define RGB_MATRIX_VAL_STEP 8
	#define RGB_MATRIX_SPD_STEP 10
	#define RGB_DISABLED_AT_STARTUP //To-do: quitarlo y sustituirlo por una animación molona
	#define RGB_MATRIX_DEFAULT_HUE 200 // Sets the default hue value, if none has been set
	#define RGB_MATRIX_DEFAULT_SAT 255 // Sets the default saturation value, if none has been set
	#define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
	#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_no_effect  //RGB will be handled manually (custom effect called "no_effect")

    //disable all effects
    #define DISABLE_RGB_MATRIX_SOLID_COLOR
	#define DISABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
	#define DISABLE_RGB_MATRIX_ALPHAS_MODS
	#define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN
	#define DISABLE_RGB_MATRIX_BREATHING
	#define DISABLE_RGB_MATRIX_BAND_SAT
	#define DISABLE_RGB_MATRIX_BAND_VAL
	#define DISABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
	#define DISABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
	#define DISABLE_RGB_MATRIX_BAND_SPIRAL_SAT
	#define DISABLE_RGB_MATRIX_BAND_SPIRAL_VAL
	#define DISABLE_RGB_MATRIX_CYCLE_ALL
	#define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
	#define DISABLE_RGB_MATRIX_CYCLE_UP_DOWN
	#define DISABLE_RGB_MATRIX_CYCLE_OUT_IN
	#define DISABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
	#define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define DISABLE_RGB_MATRIX_HUE_BREATHING
    #define DISABLE_RGB_MATRIX_HUE_PENDULUM
    #define DISABLE_RGB_MATRIX_HUE_WAVE
	#define DISABLE_RGB_MATRIX_DUAL_BEACON
	#define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
	#define DISABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define DISABLE_RGB_MATRIX_PIXEL_FRACTAL
	#define DISABLE_RGB_MATRIX_RAINBOW_BEACON
	#define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
	#define DISABLE_RGB_MATRIX_RAINDROPS
    #define DISABLE_RGB_MATRIX_PIXEL_FLOW
    #define DISABLE_RGB_MATRIX_PIXEL_RAIN
	#define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
	#define DISABLE_RGB_MATRIX_TYPING_HEATMAP
	#define DISABLE_RGB_MATRIX_DIGITAL_RAIN
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
	#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
	#define DISABLE_RGB_MATRIX_SPLASH
	#define DISABLE_RGB_MATRIX_MULTISPLASH
	#define DISABLE_RGB_MATRIX_SOLID_SPLASH
	#define DISABLE_RGB_MATRIX_SOLID_MULTISPLASH
    #define RGBLIGHT_DISABLE_KEYCODES
    #undef RGB_MATRIX_KEYPRESSES
    #undef RGB_MATRIX_KEYRELEASES
    #undef RGB_MATRIX_FRAMEBUFFER_EFFECTS

//ojito que con esto estoy experimentando
//https://docs.qmk.fm/#/feature_rgb_matrix?id=additional-configh-options
  //  #define RGB_MATRIX_KEYPRESSES
   // #define SPLIT_TRANSPORT_MIRROR

#endif



//To reduce file size
#define RETRO_TAPPING
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define NO_ACTION_ONESHOT
#define NO_ACTION_TAPPING
#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_FORCE_HOLD
#define DISABLE_LEADER


//Even further (export release) (comment all during development)
#ifndef NO_DEBUG
#define NO_DEBUG
#endif
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
#define NO_PRINT
#endif
