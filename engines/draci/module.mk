MODULE := engines/draci
 
MODULE_OBJS := \
	draci.o \
	detection.o \
	barchive.o \
	script.o \
	font.o \
	saveload.o \
	sprite.o \
	screen.o \
	surface.o \
	mouse.o \
	game.o \
	animation.o
 
MODULE_DIRS += \
	engines/draci
 
# This module can be built as a plugin
ifeq ($(ENABLE_DRACI), DYNAMIC_PLUGIN)
PLUGIN := 1
endif
 
# Include common rules 
include $(srcdir)/rules.mk
