MCU = STM32F411
USE_FPU = YES
STM32_BOOTLOADER_ADDRESS = 0x1FFF0000

#-------------------------------------------------

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = lite     # Virtual DIP switch configuration
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
NKRO_ENABLE = no            # USB Nkey Rollover
BACKLIGHT_ENABLE = yes      # Enable keyboard backlight functionality on B7 by default
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
UNICODE_ENABLE = yes        # Unicode
AUDIO_ENABLE = no           # Audio output
ENCODER_ENABLE = yes
MIDI_ENABLE = no
STENO_ENABLE = no


# Allow for single-side builds to be overridden in keymaps
SPLIT_KEYBOARD ?= yes

SERIAL_DRIVER = usart

BACKLIGHT_DRIVER = pwm

EEPROM_DRIVER = spi

WS2812_DRIVER = pwm
CIE1931_CURVE = yes

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

# Disable LTO as it messes with data transactions between sides
LTO_ENABLE = yes
OPT = 2

#LTO_ENABLE = no
#OPT = 0
#OPT_DEFS += -g

LUA_ENABLE = no

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS = st7789
