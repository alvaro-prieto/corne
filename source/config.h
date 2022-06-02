
#pragma once

//no need for underglow in a closed case. Also, I use foam and I don't want any overheat
#define DISABLE_UNDERGLOW
#define LOCK_KB_ON_LOCK_OS
#define TAPPING_TERM 250
#define UNICODE_SELECTED_MODES UC_WINC
#define UNICODE_CYCLE_PERSIST false
#define OVERFLOW_SMALL 15
#define MASTER_LEFT
//#define MASTER_RIGHT

#undef USE_I2C
#undef SSD1306OLED
#define USE_SERIAL_PD2

#ifdef RGB_MATRIX_ENABLE

	//RGB ANIMATIONS:
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
	#define DISABLE_RGB_MATRIX_DUAL_BEACON
	#define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL
	#define DISABLE_RGB_MATRIX_CYCLE_SPIRAL
	#define DISABLE_RGB_MATRIX_RAINBOW_BEACON
	#define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
	#define DISABLE_RGB_MATRIX_RAINDROPS
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

	//RGB CONFIGURATION:
	#define RGB_MATRIX_KEYPRESSES // reacts to keypresses
	//#define RGB_MATRIX_KEYRELEASES // reacts to keyreleases (instead of keypresses)
	//#define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
	#define RGB_DISABLE_WHEN_USB_SUSPENDED true // turn off effects when suspended
	#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
	//#define RGB_MATRIX_LED_PROCESS_LIMIT (DRIVER_LED_TOTAL + 4) / 5 // limits the number of LEDs to process in an animation per task run (increases keyboard responsiveness)
	#define RGB_MATRIX_LED_PROCESS_LIMIT DRIVER_LED_TOTAL
	#define RGB_MATRIX_LED_FLUSH_LIMIT 300 // limits in milliseconds how frequently an animation will update the LEDs. 16 (16ms) is equivalent to limiting to 60fps (increases keyboard responsiveness)
	#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120 //WARNING! limits maximum brightness of LEDs to 120 out of 255. Higher may cause the controller to crash.
	#define RGB_MATRIX_HUE_STEP 8
	#define RGB_MATRIX_SAT_STEP 8
	#define RGB_MATRIX_VAL_STEP 8
	#define RGB_MATRIX_SPD_STEP 10
	#define RGB_DISABLE_TIMEOUT 20 * 1000
	#define RGB_DISABLED_AT_STARTUP
	#define RGB_MATRIX_STARTUP_HUE 100 // Sets the default hue value, if none has been set
	#define RGB_MATRIX_STARTUP_SAT 255 // Sets the default saturation value, if none has been set
	#define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS // Sets the default brightness value, if none has been set
	#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CUSTOM_custom_effect  //RGB will be handled manually

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
#endif // !NO_DEBUG
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
#define NO_PRINT
#endif // !NO_PRINT

