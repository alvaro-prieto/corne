#Are you debugging?
COMBO_ENABLE = yes				#set it to no only if you need further memory
RGB_MATRIX_ENABLE  =  yes		#set it to no only if you need further memory
CONSOLE_ENABLE = no			# (5%) set it to no in a release version, yes only to debug
EXTRAKEY_ENABLE = yes			# (2%) required for multimedia keys in Windows

##===================
BOOTLOADER = atmel-dfu
COMMAND_ENABLE = no
TAP_DANCE_ENABLE = yes
APPLE_FN_ENABLE = yes
OLED_DRIVER_ENABLE = no
MOUSEKEY_ENABLE = no 		# Mouse keys(+4700)
BOOTMAGIC_ENABLE = no       # Virtual DIP switch configuration(+1000)
NKRO_ENABLE = no            # Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
MIDI_ENABLE = no            # MIDI controls
AUDIO_ENABLE = no           # Audio output on port C6
UNICODE_ENABLE = yes        # Unicode
BLUETOOTH_ENABLE = no       # Enable Bluetooth with the Adafruit EZ-Key HID
SWAP_HANDS_ENABLE = no      # Enable one-hand typing
RGBLIGHT_ENABLE    = no
MAGIC_ENABLE = no
#EXTRAFLAGS += -flto
EXTRAFLAGS = -flto -mcall-prologues
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
NO_USB_STARTUP_CHECK = yes
RGB_MATRIX_CUSTOM_USER = yes
STENO_ENABLE = no
LTO_ENABLE = yes
OLED_ENABLE = no

##===================
#To save further bytes (doesn't seem to do anything at all)
DYNAMIC_MACRO_ENABLE = no
KEY_LOCK_ENABLE = no
VELOCIKEY_ENABLE = no
TERMINAL_ENABLE = no
AUTO_SHIFT_ENABLE = no
POINTING_DEVICE_ENABLE = no
RAW_ENABLE = no
SEQUENCER_ENABLE = no
WPM_ENABLE = no
AUTO_SHIFT_MODIFIERS = no
VIA_ENABLE = no
SLEEP_LED_ENABLE = no
DYNAMIC_KEYMAP_ENABLE = no
DYNAMIC_KEYMAP_MACRO_ENABLE = no
DYNAMIC_KEYMAP_CUSTOM_FN_ENABLE = no
DYNAMIC_KEYMAP_LAYER_COUNT = no
LED_MATRIX_CUSTOM_KB = no
RGB_MATRIX_CUSTOM_KB = no
ENCODER_ENABLE = no
LEADER_ENABLE = no
ENCODER_MAP_ENABLE = no
KEYLOGGER_ENABLE = no
UNICODEMAP_ENABLE = no
VARIABLE_TRACE = no
FAUXCLICKY_ENABLE = no
HD44780_ENABLE = no


