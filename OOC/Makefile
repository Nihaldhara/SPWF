# Generic GNUMakefile
﻿MAKEFILE = gnu
CC = gcc

#debug options
DOPT = -Wall -g -Wno-unused-variable
#release options
ROPT = -Wall -o3 -Wno-unused-variable -D NDEBUG
#opt will get the release/debug config for the compiler
# change to $(DOPT) to produce debugable executable
OPT = $(ROPT)
#release lib name
RLIB = libOOC.a
#debug lib name
DLIB = libOOC_d.a

# Source files
SRC = $(wildcard *.c)

# Headers Files
HDR = $(wildcard *.h)

#object files for release version
ROBJ = $(SRC:.c=.o)

#object files for debug version
DOBJ = $(SRC:.c=_d.o)

# List of the path to use when looking up for libraries
# modify path to match your target architecture ( x86 or x64), so it will link to the good library 
LIBPATH = 

# recipes

# ------------
# release rules

all: $(RLIB)

$(RLIB): $(ROBJ) $(HDR)
	$(AR) $(AR_ARGS) $@ $(ROBJ)

%.o : %.c $(HDR)
	$(CC) $(ROPT) $(INCPATH) -c $< -o $@ $(LIBS)

# ------------
# Debug rule

debug: $(DLIB)

$(DLIB): $(DOBJ) $(HDR)
	$(AR) $(AR_ARGS) $@ $(DOBJ)

%_d.o : %.c $(HDR)
	$(CC) $(DOPT) $(INCPATH)  -c $< -o $@ $(LIBS)
	
# -----------
# clean rule

clean:
	@echo "Suppression des fichiers objets"
	@rm $(ROBJ) $(DOBJ) 2>/dev/null || true

# -----------
# rebuild rule

mrproper: clean
	@echo "Nettoyage complet"
	@rm $(RLIB) $(DLIB) 2>/dev/null || true

# memo internal macro
# $@ --> The file name of the target of the rule.
# $^ --> he names of all the prerequisites, with spaces between them.
# $< The name of the first prerequisite.
