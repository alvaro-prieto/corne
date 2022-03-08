//#ifdef RGBLIGHT_ENABLE
	#pragma once
	#define FIRST_UNDERGLOW 0
	#define LAST_UNDERGLOW 5
	#define FIRST_RGB 6
	#define LAST_RGB 26
	#define NOTIFICATION_TIMEOUT 5
	#define NUMBER_OF_KEYS LAST_RGB - FIRST_RGB + 1
	#define BRIGHTNESS_MULTIPLIER (double) RGB_MATRIX_MAXIMUM_BRIGHTNESS / 255.0
	#define BRM BRIGHTNESS_MULTIPLIER
	#define MAX_INDICATOR_LENGTH 20
	#define MAX_NUMBER_OF_INDICATORS 5

	//enum value must be the index of the first led group
	enum rgb_indicator{
		RGB_NONE = -1,
		RGB_HYPER_LOCK = 0,
		RGB_CAPS_LOCK = 1,
		RGB_SHIFT = 2,
		RGB_EXTRAS = 3,
		RGB_HYPER_HOLD = 6,
	};

	enum rgb_map{
		RGB_NO_THEME = -2,
		RGB_NO_NOTIFICATION = -1,
		RGB_WINDOWS_NOTIFICATION = 0,
		RGB_MAC_NOTIFICATION,
		RGB_LOCK_NOTIFICATION,
		RGB_THEME_1,
		RGB_THEME_2,
		RGB_LANG_ES,
		RGB_LANG_EN,

	};

	void init_rgb( void );
	void reset_rgb( void ) ;
	void set_rgb_notification( enum rgb_map indicator );
	void set_rgb_indicator( enum rgb_indicator, bool on );
	void update_rgb_state(void);
	void rgb_hibernate( bool );
	void rgb_rotate_theme( int direction );
	void rgb_matrix_suspend_state_changed( bool suspend );


//#endif
