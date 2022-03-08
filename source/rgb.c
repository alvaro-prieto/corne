#ifdef RGB_MATRIX_ENABLE

	#include "rgb.h"
	#include "handlers.h"

	/*

	  .-----------------------------------------------------------.
	  |   24    |   23    |   18    |   17    |    10   |    9    |
	  |---------+--------[2]--------+--------[1]-------[0]--------|
	  |   25    |   22    |   19    |   16    |    11   |    8    |
	  |---------+--------[3]--------+---------+---------+---------|
	  |   26    |   21    |   20    |   15    |    12   |    7    |
	  `---------+---------+---------+--------[4]-------[5]--------+---------.
	                                           \   14   |   13    |    6    |
	                                            `---------------------------´
	*/


	//Single LED color definition
	typedef struct{
		unsigned char R;
		unsigned char G;
		unsigned char B;
	} rgb;


	//Used as node in a notification led pattern. It contains its location,
	//color, and next LED node in the pattern
	typedef struct {
		unsigned char number; //add 100 if it located in the right side (slave). 200 in both sides
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char next;
	} rgb_led;

	//state variables
	int current_theme = RGB_NO_THEME;
	int current_theme_index = 0;
	int current_notification = RGB_NO_NOTIFICATION;
	int notification_countdown = 0;
	bool rgb_on = false;  //this flag let us know if any rgb is enabled or not
	unsigned char number_of_active_indicators = 0;
	enum rgb_indicator active_indicators[ MAX_NUMBER_OF_INDICATORS ];
	bool rgb_dirty = false; //pending changes to update

	//wheel of themes that the user can iterate through
	const int wheel_of_themes [] ={
		RGB_NO_THEME,
		RGB_MAC_NOTIFICATION,
		RGB_LOCK_NOTIFICATION,
		RGB_THEME_1,
		RGB_THEME_2
	};
	const int NUMBER_OF_THEMES = sizeof(wheel_of_themes) / sizeof(wheel_of_themes[0]);


	//═══════════════════════════════════════════════════════════════
	//	THEMES AND NOTIFICATIONS
	//═══════════════════════════════════════════════════════════════

	//I made this little tool to create RGB Themes
	//https://codepen.io/alvaro-prieto/full/gOLrwKm

	//It is important to save some RAM, I had keyboard irresponsiveness issues while adding
	//too many themes since they were stored initially in the Data Space, so I will use
	//PROGMEM to store them in the Program Space and save some memory

	//rgb themes[][NUMBER_OF_KEYS]  ={
	static const rgb themes[][NUMBER_OF_KEYS] PROGMEM ={

		//0 - RGB_WINDOWS_NOTIFICATION (windows logo)
		{{0x00,0x00,0x00},{0x12,0x14,0x00}, {0x9e,0x94,0x00}, {0x01,0x98,0x3b},
		{0x75,0x00,0x00}, {0x01,0x26,0xad}, {0x01,0x07,0x28}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}, {0x00,0x0b,0x2e}, {0x00,0x11,0x42}, {0x24,0x00,0x00},
		{0x0f,0x02,0x00}, {0x00,0x01,0x14}, {0x00,0x00,0x00}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}},

		//1 - RGB_MAC_NOTIFICATION (white)
		//["#ffffff","#d4d4d4","#737373","#383838","#212121","#4d4d4d","#8f8f8f","#e8e8e8","#787878","#383838","#292929","#0f0f0f","#000000","#121212","#141414","#000000","#000000","#000000","#000000","#000000","#000000"]
		{{0xff,0xff,0xff},{0xd4,0xd4,0xd4}, {0x73,0x73,0x73}, {0x38,0x38,0x38},
		{0x21,0x21,0x21}, {0x4d,0x4d,0x4d}, {0x8f,0x8f,0x8f}, {0xe8,0xe8,0xe8},
		{0x78,0x78,0x78}, {0x38,0x38,0x38}, {0x29,0x29,0x29}, {0x0f,0x0f,0x0f},
		{0x00,0x00,0x00}, {0x12,0x12,0x12}, {0x14,0x14,0x14}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}},

		//2 - RGB_LOCK_NOTIFICATION (purple)
		//["#170038","#350085","#1f00b8","#0108d5","#200094","#270094","#350075","#1c003d","#190137","#290165","#25006b","#2d0070","#190042","#0c001f","#0b001f","#000000","#000000","#100029","#000000","#000000","#000000"]
		{{0x17,0x00,0x38},{0x35,0x00,0x85}, {0x1f,0x00,0xb8}, {0x01,0x08,0xd5},
		{0x20,0x00,0x94}, {0x27,0x00,0x94}, {0x35,0x00,0x75}, {0x1c,0x00,0x3d},
		{0x19,0x01,0x37}, {0x29,0x01,0x65}, {0x25,0x00,0x6b}, {0x2d,0x00,0x70},
		{0x19,0x00,0x42}, {0x0c,0x00,0x1f}, {0x0b,0x00,0x1f}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}, {0x10,0x00,0x29}, {0x00,0x00,0x00}, {0x00,0x00,0x00},
		{0x00,0x00,0x00}},

		//3 - RGB_THEME_1  (yellow)
		//["#ffffff","#b6992f","#c26700","#bd4200","#8e4c01","#bd6e00","#ffdd00","#eee6bf","#98957c","#a88400","#755000","#563701","#2d1b01","#462901","#614200","#241100","#241700","#241500","#0e0901","#0f0800","#000000"]
		{{0xff,0xff,0xff},{0xb6,0x99,0x2f}, {0xc2,0x67,0x00}, {0xbd,0x42,0x00},
		{0x8e,0x4c,0x01}, {0xbd,0x6e,0x00}, {0xff,0xdd,0x00}, {0xee,0xe6,0xbf},
		{0x98,0x95,0x7c}, {0xa8,0x84,0x00}, {0x75,0x50,0x00}, {0x56,0x37,0x01},
		{0x2d,0x1b,0x01}, {0x46,0x29,0x01}, {0x61,0x42,0x00}, {0x24,0x11,0x00},
		{0x24,0x17,0x00}, {0x24,0x15,0x00}, {0x0e,0x09,0x01}, {0x0f,0x08,0x00},
		{0x00,0x00,0x00}},

		//4 - RGB_THEME_2 (rainbow)
		//["#f60909","#a38d00","#006b1b","#180868","#300693","#007375","#74c200","#faa005","#f5f901","#1ba300","#005766","#240066","#1d0061","#004152","#008a4e","#015641","#013037","#001442","#000f33","#001d33","#002e23"]
		{{0xf6,0x09,0x09},{0xa3,0x8d,0x00}, {0x00,0x6b,0x1b}, {0x18,0x08,0x68},
		{0x30,0x06,0x93}, {0x00,0x73,0x75}, {0x74,0xc2,0x00}, {0xfa,0xa0,0x05},
		{0xf5,0xf9,0x01}, {0x1b,0xa3,0x00}, {0x00,0x57,0x66}, {0x24,0x00,0x66},
		{0x1d,0x00,0x61}, {0x00,0x41,0x52}, {0x00,0x8a,0x4e}, {0x01,0x56,0x41},
		{0x01,0x30,0x37}, {0x00,0x14,0x42}, {0x00,0x0f,0x33}, {0x00,0x1d,0x33},
		{0x00,0x2e,0x23}},

		//5 - RGB_LANG_ES (spanish flag)
		//["#bc0101","#f2a602","#e9a001","#d60000","#d60000","#ffdd00","#ffdd00","#bd0000","#8f0000","#bd9d00","#cb9f01","#940000","#6b0000","#946c00","#987701","#614c00","#704d00","#4d0000","#2d0101","#3d2e00","#3d2e00"]
		//["#bc0101","#f2a602","#e9a001","#d60000","#a30000","#ffc800","#a88400","#8e0101","#6a0101","#805e00","#987001","#570000","#420000","#613d00","#654501","#2e1f00","#332000","#290000","#0e0101","#181101","#191000"]
		{{0xbc,0x01,0x01},{0xf2,0xa6,0x02}, {0xe9,0xa0,0x01}, {0xd6,0x00,0x00},
		{0xa3,0x00,0x00}, {0xff,0xc8,0x00}, {0xa8,0x84,0x00}, {0x8e,0x01,0x01},
		{0x6a,0x01,0x01}, {0x80,0x5e,0x00}, {0x98,0x70,0x01}, {0x57,0x00,0x00},
		{0x42,0x00,0x00}, {0x61,0x3d,0x00}, {0x65,0x45,0x01}, {0x2e,0x1f,0x00},
		{0x33,0x20,0x00}, {0x29,0x00,0x00}, {0x0e,0x01,0x01}, {0x18,0x11,0x01},
		{0x19,0x10,0x00}},

		//6 - RGB_LANG_EN (english flag)
		//["#ff0000","#ff0000","#ffffff","#ababab","#b0b0b0","#bd0000","#ff0000","#bd0000","#001eff","#0011ff","#990000","#750000","#6b0000","#1100ff","#0208bb","#020080","#001194","#0c00b3","#001061","#0d0165","#070137"]
		{{0xff,0x00,0x00},{0xff,0x00,0x00}, {0xff,0xff,0xff}, {0xab,0xab,0xab},
		{0xb0,0xb0,0xb0}, {0xbd,0x00,0x00}, {0xff,0x00,0x00}, {0xbd,0x00,0x00},
		{0x00,0x1e,0xff}, {0x00,0x11,0xff}, {0x99,0x00,0x00}, {0x75,0x00,0x00},
		{0x6b,0x00,0x00}, {0x11,0x00,0xff}, {0x02,0x08,0xbb}, {0x02,0x00,0x80},
		{0x00,0x11,0x94}, {0x0c,0x00,0xb3}, {0x00,0x10,0x61}, {0x0d,0x01,0x65},
		{0x07,0x01,0x37}},

	};

	//═══════════════════════════════════════════════════════════════
	//	INDICATORS
	//═══════════════════════════════════════════════════════════════

	//First value is corresponds to LED positioning according to the illustration above.
	//value < 100  = left
	//value >= 100 && < 200 = right
	//value >= 200 = both sides

	static const rgb_led leds[] PROGMEM ={
		//0 - RGB_HYPER_LOCK
		{126, 0xff * BRM, 0x00 * BRM, 0x90 * BRM, 0},
		//1 - RGB_CAPS_LOCK
		{126, 0x21 * BRM, 0xd1 * BRM, 0xc8 * BRM, 0},
		//2 - RGB_SHIFT
		{126, 0x00 * BRM, 0x10 * BRM, 0x65 * BRM, 0},
		//3 - RGB_EXTRAS
		{214, 0xbb * BRM, 0x15 * BRM, 0x00 * BRM, 4},
		{213, 0xbb * BRM, 0x15 * BRM, 0x00 * BRM, 5},
		{206, 0xbb * BRM, 0x15 * BRM, 0x00 * BRM, 0},
		//6 - RGB_HYPER_HOLD
		{214, 0xff * BRM, 0x00 * BRM, 0x90 * BRM, 7},
		{213, 0xff * BRM, 0x00 * BRM, 0x90 * BRM, 8},
		{206, 0xff * BRM, 0x00 * BRM, 0x90 * BRM, 9},
		{126, 0x36 * BRM, 0x00 * BRM, 0x2c * BRM, 0}
	};

	//═══════════════════════════════════════════════════════════════
	//	FUNCTIONS
	//═══════════════════════════════════════════════════════════════

	//turn leds black (clear)
	void turn_off_all_leds(void){
		rgb_matrix_set_color_all(0, 0, 0);
	}

	//draw a theme / notification. Brightness from 0 to 255 (be careful with intensity)
	void draw_rgb_map(enum rgb_map theme_index, double brightness){
		brightness = brightness > RGB_MATRIX_MAXIMUM_BRIGHTNESS ? RGB_MATRIX_MAXIMUM_BRIGHTNESS : brightness;
		double br = brightness / 255.0;

		//Old code, (used when themes was not PROGMEM)
		//I found out that is very important to save RAM
		//since I was experiencing keyboard unresponsiveness
		//when too many themes were stored in the Data Space (RAM)

		/*
		rgb *theme = themes[ theme_index ];
		rgb led;
		for(int i=0; i<NUMBER_OF_KEYS; i++){
			led = theme[i];
			rgb_matrix_set_color( i + FIRST_RGB , led.R * br, led.G * br, led.B * br );
		}
		*/

		//New implementation for PROGMEM themes. This way is not the safest one
		//to access data, using its size as offset (+0, +1, +2), but it works.
		//A safer way of doing the same kind of task is shown in "dance_finished" (dances.c)
		uintptr_t rgb_led;
		for(int i=0; i<NUMBER_OF_KEYS; i++){
			rgb_led = (uintptr_t) &(themes[theme_index][i]) ;
			rgb_matrix_set_color( i + FIRST_RGB ,
				pgm_read_byte( rgb_led + 0 ) * br,
				pgm_read_byte( rgb_led + 1 ) * br,
				pgm_read_byte( rgb_led + 2 ) * br
			);
		}
	}

	//draw current theme
	void draw_current_theme(enum rgb_map theme_index){
		//uprintf("Theme is going to be drawn, current brightness: %d \n",rgb_matrix_get_val());
		draw_rgb_map( theme_index, 100 );
	}

	//draw current notification
	void draw_current_notification(void){
		draw_rgb_map( current_notification,  RGB_MATRIX_MAXIMUM_BRIGHTNESS);
	}

	//function that can be called from other files to activate a
	//temporal visual notification (such as OS swap)
	void set_rgb_notification(enum rgb_map theme){
		notification_countdown =  NOTIFICATION_TIMEOUT;
		current_notification = theme;
		rgb_dirty = true;
		update_rgb_state();
	}

	//internal function to add an indicator to the drawing queue
	void add_indicator(enum rgb_indicator indicator){
		for(int i=0; i<MAX_NUMBER_OF_INDICATORS; i++){
			if(active_indicators[i] == RGB_NONE){
				active_indicators[i] = indicator;
				number_of_active_indicators++;
				return;
			}
		}
	}

	//internal function to remove an indicator from the drawing queue
	void remove_indicator(enum rgb_indicator indicator){
		for(int i=0; i<MAX_NUMBER_OF_INDICATORS; i++){
			if(active_indicators[i] == indicator){
				active_indicators[i] = RGB_NONE;
				number_of_active_indicators--;
				return;
			}
		}
	}

	//This function is used to determine if the LED system
	//should be switched off or kept on.
	void update_rgb_state(void){
		bool rgb_in_use = (
				current_notification != RGB_NO_NOTIFICATION ||
				current_theme != RGB_NO_THEME ||
				number_of_active_indicators > 0
			);
		if(rgb_in_use != rgb_on){
			rgb_on ? rgb_matrix_disable() : rgb_matrix_enable();
			rgb_on = rgb_in_use;
			//rgb_on ? uprintf("Enable RGB \n") : uprintf("Disable RGB \n");
		}
	}

	//function that can be called from other files to enable a visual indicator,
	//such as caps lock, hyper, ...
	void set_rgb_indicator(enum rgb_indicator indicator, bool on){
		if(on){
			if(number_of_active_indicators < MAX_NUMBER_OF_INDICATORS){
				add_indicator( indicator );
			}
		}else{
			if(number_of_active_indicators > 0){
				remove_indicator( indicator );
			}
		}
		rgb_dirty = true;
		update_rgb_state();
	}

	//function that can be called from other files to change the current theme.
	//direction can be either 1 or -1
	void rgb_rotate_theme( int direction ){
		current_theme_index += direction;
		if(current_theme_index < 0){
			current_theme_index = NUMBER_OF_THEMES - 1;
		}
		if(current_theme_index >=  NUMBER_OF_THEMES){
			current_theme_index = 0;
		}
		current_theme = wheel_of_themes[ current_theme_index ];
		rgb_dirty = true;
		update_rgb_state();
	}


	//this function is used to draw an indicator, that can be compound of several LEDs
	void draw_indicator(enum rgb_indicator indicator){
		unsigned char iteration = 0, index, adjust;
		uintptr_t ledAddress;
		while( iteration < MAX_INDICATOR_LENGTH && (iteration++ == 0 || indicator > 0) ){
			ledAddress = (uintptr_t) &(leds[ indicator ]);
			index = pgm_read_byte( ledAddress + 0 ); //number
			adjust = index >= 200 ? 200 : index >= 100 ? 100 : 0;
			if( (is_master &&  (index < 100 || index >= 200) ) ||
				(!is_master && index >= 100)){
				rgb_matrix_set_color( index - adjust,
					pgm_read_byte(ledAddress + 1),	//R
					pgm_read_byte(ledAddress + 2),	//G
					pgm_read_byte(ledAddress + 3)	//B
				);
			}
			indicator = pgm_read_byte( ledAddress + 4 ); //next
		}
	}

	//this function is used to draw all active indicators
	void draw_indicators(void){
		int pending = number_of_active_indicators;
		for(int i=0; i<MAX_NUMBER_OF_INDICATORS && pending>0; i++){
			if(active_indicators[i] != RGB_NONE){
				draw_indicator( active_indicators[i] );
				pending--;
			}
		}
	}

	//remove all active indicators
	void reset_indicators(void){
		number_of_active_indicators = 0;
		for(int i=0; i<MAX_NUMBER_OF_INDICATORS; i++){
			active_indicators[i] = RGB_NONE;
		}
	}

	//fired when the RESET_RGB key is pressed
	void reset_rgb(void){
		/*
		not required in this implementation
		eeconfig_update_rgb_matrix_default();
		//RGB_MATRIX_MAXIMUM_BRIGHTNESS
		//rgb_matrix_sethsv(HSV_OFF);
		rgb_matrix_sethsv(0,255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
		rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
		rgb_matrix_enable();
		*/
	}


	//function that is executed when the keyboard is ready
	void init_rgb(void){
		reset_indicators();
		#ifdef DISABLE_UNDERGLOW
			//no underlight required since my keyboard has an opaque case
			rgb_matrix_set_flags(LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER);
  		#endif
	}

	//function that is executed when the keyboard is ready
	void matrix_init_user(void) {
		rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_custom_effect);
		rgb_matrix_sethsv(RGB_MATRIX_STARTUP_HUE, RGB_MATRIX_STARTUP_SAT, RGB_MATRIX_STARTUP_VAL);
		rgb_on = rgb_dirty = current_theme != RGB_NO_THEME;
		rgb_dirty ? rgb_matrix_enable() : rgb_matrix_disable();

	}

	//rgb_matrix.c has been modified as stated in notes.txt to be able to detect
	//when the keyboard changes its RGB suspend state.
	void rgb_matrix_suspend_state_changed( bool suspend ){
        if(kb_lock) return;
		if(suspend){
			rgb_dirty = true;
		}else{
            update_rgb_state();
		}
	}




	//Main RGB loop. This function will be executed only when rgb_matrix is enabled
	int tick = 0;
	void rgb_matrix_indicators_user(void) {

		//if( !(tick++ % 10) ) uprintf(".");
		if( current_notification != RGB_NO_NOTIFICATION ){
			if(!--notification_countdown){
				current_notification = RGB_NO_NOTIFICATION;
				rgb_dirty = true;
				//uprintf("Notification is over \n");
				update_rgb_state();
			}
		}

		if( rgb_dirty ){
			//uprintf("-");
			turn_off_all_leds();
			if( current_notification != RGB_NO_NOTIFICATION ){
				draw_current_notification();
			}else if( current_theme != RGB_NO_THEME ){
				draw_current_theme( current_theme );
			}
			draw_indicators();
			rgb_dirty = false;
		}

	}

#endif

