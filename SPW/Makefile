# Generic GNUMakefile
﻿MAKEFILE = gnu

# Compiler
CC = gcc
#debug options
DOPT = -Wall -g -Wno-unused-variable
#release options
ROPT = -Wall -o3 -Wno-unused-variable #-D NDEBUG
#release binary name
RBIN = spw.bin
#debug binary name
DBIN = spw_d.bin

# Create the list of directories
DIRS = ./ ./Input ./Scene ./Utils ./GameObject \
./GameObject/Camera ./GameObject/Collectable ./GameObject/Enemy ./GameObject/SceneManager \
./GameObject/Terrain ./GameObject/UI ./GameObject/UI/LevelUI ./GameObject/UI/TitleUI \

# Create a list of *.c sources in DIRS
SRC = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
# Headers Files
HDR = $(wildcard *.h)

#object files for release version
ROBJ = $(SRC:.c=.o)
#object files for debug version
DOBJ = $(SRC:.c=_d.o)

# folders for local static libs
LIB_FOLDS = ../OOC/ ../PlatformerEngine/ ../RenderingEngine/
#lib files for release version
RLIB = libOOC.a libPE.a libRE.a
RLIB_W_PATH = ../OOC/libOOC.a ../PlatformerEngine/libPE.a ../RenderingEngine/libRE.a 
#lib files for debug version
DLIB = $(RLIB:.a=_d.a)
DLIB_W_PATH = ../OOC/libOOC_d.a ../PlatformerEngine/libPE.a ../RenderingEngine/libRE.a 

# list of libraries to be linked dynamically
EXT_LIBS = -lm -lSDL2 `sdl2-config --libs` -lSDL2_image -fopenmp -lSDL2_ttf
LOC_LIBS = -L../RenderingEngine -lRE -L../PlatformerEngine/ -lPE -L../OOC/ -lOOC



# ------------
# release rules

.c.o:
	$(CC) $(ROPT) -c $(CFLAGS) $<

all: $(RBIN)

$(RBIN): $(ROBJ)
	$(CC) $(ROPT) -o $@ $(ROBJ) $(RLIB_W_PATH) $(EXT_LIBS) $(LOC_LIBS)

%.o : %.c $(HDR)
	$(CC) $(ROPT) -c $< -o $@ 


# ------------
# Debug rule

debug: $(DBIN)

$(DBIN): $(DOBJ) $(HDR)
	$(CC) $(DOPT) -o $@ $(DOBJ) $(DLIB_W_PATH) $(EXT_LIBS) $(LOC_LIBS)

%_d.o : %.c $(HDR)
	$(CC) $(DOPT) -c $< -o $@ 


# -----------
# clean rule

clean:
	@echo "Suppression des fichiers objets locaux"
	@rm $(ROBJ) $(DOBJ) 2>/dev/null || true

# -----------
# super clean rule

mrproper: clean
	@echo "Nettoyage complet"
	@rm $(RBIN) $(DBIN) $(RLIB) $(DLIB) 2>/dev/null || true
	@make clean -C ../OOC/

# memo internal macro
# $@ --> The file name of the target of the rule.
# $^ --> The names of all the prerequisites, with spaces between them.
# $< The name of the first prerequisite.
