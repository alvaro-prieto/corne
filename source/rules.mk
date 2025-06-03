#Are you debugging?
#–––––––––––––––––––––
CONSOLE_ENABLE = yes # to be able to use xprintf()
RGB_DEBUG = no  # to debug slave

# Aditional .c files
#–––––––––––––––––––––
SRC +=	$(USER_PATH)/sync.c \
		$(USER_PATH)/handlers_before.c \
		$(USER_PATH)/handlers_smart.c \
		$(USER_PATH)/handlers_mods.c \
		$(USER_PATH)/handlers_custom.c \
		$(USER_PATH)/handlers.c \
		$(USER_PATH)/shared.c \
		$(USER_PATH)/main.c
ifeq ($(RGB_MATRIX_ENABLE),yes)
	SRC += $(USER_PATH)/rgb.c
endif
SRC +=	$(USER_PATH)/keydefs.c

# Basic config
#–––––––––––––––––––––
INTROSPECTION_KEYMAP_C = $(USER_PATH)/keymap.c
UNICODE_ENABLE = yes
TAP_DANCE_ENABLE = yes
APPLE_KEYBOARD = yes
RGB_MATRIX_CUSTOM_USER = yes
COMBO_ENABLE = yes
EXTRAKEY_ENABLE = yes
DEFERRED_EXEC_ENABLE = yes
NKRO_ENABLE = no
MAGIC_ENABLE = no
BOOTMAGIC_ENABLE = no
KEYBOARD_SHARED_EP = no
TIMER_USE_32BIT = yes

#–––––––––––––––––––––
LTO_ENABLE = yes
OLED_ENABLE = no
OLED_DRIVER_ENABLE = no
BACKLIGHT_ENABLE = no
BLUETOOTH_ENABLE = no
SWAP_HANDS_ENABLE = no
RGBLIGHT_ENABLE    = no
MOUSEKEY_ENABLE = no
STENO_ENABLE = no

ifeq ($(strip $(RGB_DEBUG)), yes)
    OPT_DEFS += -DRGB_DEBUG
endif
