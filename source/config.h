#pragma once

//Impersonate an Apple keyboard (probably not needed anymore)
//#undef VENDOR_ID
//#undef DEVICE_VER
//#undef PRODUCT_ID
//#undef MANUFACTURER
//#undef MAINTAINER
//#define VENDOR_ID       0x05ac
//#define DEVICE_VER      0x0074
//#define PRODUCT_ID      0x0250
//#define MANUFACTURER    "Apple Inc."
//#define MAINTAINER      "Alvaro Prieto Lauroba"
#define TAP_HOLD_CAPS_DELAY 110

// Custom config
#define PASSWORD "pass"         // Password to unlock the keyboard (must be a basic latin string)
#define LOG_SIZE 5              // Keylog length (passwords and auto-diaeresis)
#define DISABLE_UNDERGLOW       // No need for underglow in a closed case
#define LOCK_KB_ON_BOOT 0 	    // Lock keyboard with password on boot (0 = no, 1 = yes)
#define LOCK_KB_ON_LOCK_OS 0    // Lock keyboard with password on lock (0 = no, 1 = yes)
#define OVERFLOW_SMALL 15       // Hacky way to prevent infinite loops
#define SLEEP_TIMEOUT 120 * 1000

// General
#define MASTER_LEFT
#define LAYER_STATE_16BIT
#define DEBOUNCE 25
#define TAPPING_TERM 250
#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE
#define SPLIT_TRANSACTION_IDS_USER RPC_ID_CUSTOM_SYNC

// Combos
#define COMBO_HOLD_TIME 150
#define COMBO_TERM 50
#define COMBO_SHOULD_TRIGGER
#define COMBO_STRICT_TIMER

// RGB
#ifdef RGB_MATRIX_ENABLE
	#undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
	#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120	// WARNING! 120 out of 255. Higher may cause the controller to crash
	#define RGB_MATRIX_LED_FLUSH_LIMIT 30		// Ms. High values improve keyboard responsiveness; 16 (16ms) matches a 60fps limit
	#define RGB_MATRIX_LED_PROCESS_LIMIT RGB_MATRIX_LED_COUNT
	#define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
	#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_no_effect
	#define RGB_MATRIX_HUE_STEP 8
	#define RGB_MATRIX_SAT_STEP 8
	#define RGB_MATRIX_VAL_STEP 8
	#define RGB_MATRIX_SPD_STEP 10
    #define RGB_DISABLE_WHEN_USB_SUSPENDED false
#endif

// UEFI was not detecting some of my boards
#define SPLIT_USB_DETECT
#define MASTER_LEFT
#define SPLIT_WATCHDOG_ENABLE
#define SPLIT_MAX_CONNECTION_ERRORS 10
#define SPLIT_USB_TIMEOUT 3000
#define SPLIT_WATCHDOG_TIMEOUT 3100
#define SPLIT_USB_TIMEOUT_POLL 5
#define SPLIT_CONNECTION_CHECK_TIMEOUT 10000;

// RP2040 reset double tap
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U
