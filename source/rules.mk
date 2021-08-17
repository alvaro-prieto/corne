BOOTLOADER = atmel-dfu
TAP_DANCE_ENABLE = yes
APPLE_FN_ENABLE = yes
EXTRAKEY_ENABLE = yes   	#required for multimedia keys in Windows
OLED_DRIVER_ENABLE = no
MOUSEKEY_ENABLE = no 		# Mouse keys(+4700)
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration(+1000)
NKRO_ENABLE = no            # Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
MIDI_ENABLE = no            # MIDI controls
AUDIO_ENABLE = no           # Audio output on port C6
UNICODE_ENABLE = yes        # Unicode
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
SWAP_HANDS_ENABLE = no      # Enable one-hand typing

RGBLIGHT_ENABLE    = no
RGB_MATRIX_ENABLE  =  yes  
RGB_MATRIX_CUSTOM_USER = yes 
EXTRAFLAGS += -flto

#Are you debugging? 	If so: 	REMEMBER to turn CONSOLE_ENABLE = yes
CONSOLE_ENABLE = no #no #yes 	CONSOLE_ENABLE = no in a release version
COMMAND_ENABLE = no  

#To save some extra bytes
#SPACE_CADET_ENABLE = no #en mi version de QMK no me sale rentable, quizas a futuro
GRAVE_ESC_ENABLE = no

