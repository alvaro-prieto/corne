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
RGB_MATRIX_ENABLE  =  yes		#REMEMBER: no in development! yes in release
RGB_MATRIX_CUSTOM_USER = yes		#REMEMBER: no in development! yes in release
EXTRAFLAGS += -flto

#Are you debugging?
CONSOLE_ENABLE = no		#REMEMBER:yes in development! no in a release version
COMMAND_ENABLE = no

#To save some extra bytes
#SPACE_CADET_ENABLE = no #en mi version de QMK no me sale rentable, quizas a futuro
GRAVE_ESC_ENABLE = no

#To save further bytes (doesn't seem to do anything at all, maybe in a more recent QMK version)
DYNAMIC_MACRO_ENABLE = no
KEY_LOCK_ENABLE = no
VELOCIKEY_ENABLE = no
STENO_ENABLE = no
TERMINAL_ENABLE = no
AUTO_SHIFT_ENABLE = no
POINTING_DEVICE_ENABLE = no
RAW_ENABLE = no
SEQUENCER_ENABLE = no
WPM_ENABLE = no
OLED_ENABLE = no
AUTO_SHIFT_MODIFIERS = no
LTO_ENABLE = yes
VIA_ENABLE = no

#SLEEP_LED_ENABLE = no            # Breathing sleep LED during USB suspend
