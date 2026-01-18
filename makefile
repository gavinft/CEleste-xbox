# ----------------------------
# Makefile Options
# ----------------------------

NAME = CELESTEX
ICON = icon.png
DESCRIPTION = "Celeste Classic Xbox Controller"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
