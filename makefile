# ----------------------------
# Makefile Options
# ----------------------------

NAME = RHRSolvr
ICON = icon.png
DESCRIPTION = "No more twisting your hand like a maniac!"
COMPRESSED = NO
ARCHIVED = NO

SOURCES = src/main.cpp src/mathutils.cpp src/vec3.cpp src/physics.cpp

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
