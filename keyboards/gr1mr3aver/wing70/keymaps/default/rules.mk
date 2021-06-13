
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS = st7789

#Debug console etc.
CONSOLE_ENABLE = yes    	# Console for debug
COMMAND_ENABLE = no     # Commands for debug and configuration
VIA_ENABLE = no		# support for via dynamic keymaps

SRC += graphics/gif/gif.c \
		features/display/display.c \
		features/config_menu/menu.c \
		games/pong/pong.c \
		fonts/thintel15.c

OPT_DEFS += -lprintf_flt
