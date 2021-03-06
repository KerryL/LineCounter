# makefile.inc (LineCounter)
# This file contains all of the environment definitions
# common to each of the makefiles within the project. 
#

# Include directories that are not already on the path
# DO NOT include the -I prefix to these paths - it will
# be added automatically
INCDIRS_TEMP = \
	

INCDIRS = $(addprefix -I,$(INCDIRS_TEMP))

# Library directories that are not already on the path
# DO NOT include the -L prefix to these paths - it will
# be added automatically
LIBDIRS_TEMP = \
	

LIBDIRS = $(addprefix -L,$(LIBDIRS_TEMP))

# Libraries to link against
# DO NOT include the -l prefix to these libraries - it
# will be added automatically
LIBS_TEMP = \
	

LIBS = $(addprefix -l,$(LIBS_TEMP))

# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -g -Wall -Werror -Wextra $(INCDIRS) `wx-config --cppflags` -DwxUSE_GUI=0

# Linker flags
LDFLAGS = $(LIBDIRS) $(LIBS) `wx-config --libs`

# Object file output directory
OBJDIR = $(CURDIR)/.obj/

# Binary file output directory
BINDIR = $(CURDIR)/bin/

# Method for creating directories
MKDIR = mkdir -p

# Method for removing files
RM = rm -f
