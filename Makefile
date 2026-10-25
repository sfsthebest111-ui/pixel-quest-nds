# Pixel Quest - Nintendo DS homebrew
# Build with devkitPro devkitARM + libnds.

.SUFFIXES:

ifeq ($(strip $(DEVKITPRO)),)
$(error "DEVKITPRO is not set. Install devkitPro and Nintendo DS tools.")
endif

TARGET := pixel_quest
BUILD := build
SOURCES := source
DATA :=
INCLUDES :=

include $(DEVKITPRO)/libnds/Makefile
