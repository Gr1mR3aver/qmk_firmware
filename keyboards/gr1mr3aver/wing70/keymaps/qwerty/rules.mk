#Debug console etc.
CONSOLE_ENABLE = no    	# Console for debug
COMMAND_ENABLE = no     # Commands for debug and configuration
VIA_ENABLE = yes	# support for via dynamic keymaps

SRC += graphics/gif/gif.c \
		features/display/display.c \
		features/config_menu/menu.c \
		games/pong/pong.c \
		fonts/thintel15.c
