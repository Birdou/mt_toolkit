
################################
# Executable name (without extension)
FILE=prog
# Set some value if using SDL2
USING_SDL2=True

# Main file name (where the main function is)
MAINFILE=main

# Source files extension (.c, .cpp)
SOURCE_EXT=.cpp
# Header files extension (.h, .hpp)
HEADER_EXT=.h .hpp

# Folder that contains the source files (.c, .cpp)
SRCFOLDER=src
# Folder that contains the header files (.h, .hpp)
INCFOLDER=include
# Folder where the binary files (.o) will be compiled
OBJFOLDER=objects

# Environment defines
DEFINES=DEBUG

# Compiler that will be used to build binaries and link the executable (gcc, g++, c++...)
CC=g++

# Flags that will be used when compiling binaries
COMPILATION_FLAGS=-Wall -Wextra -pedantic -std=c++11 -g
# Flags that will be used when linking the executable
LINKER_FLAGS=
################################

# SDL2 flags
ifdef USING_SDL2
ifeq ($(OS),Windows_NT)
SDL_COMP_FLAGS=-IC:/MinGW/include/SDL2
SDL_LINK_FLAGS=-LC:/MinGW/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
else
SDL_COMP_FLAGS=-I/usr/include/SDL2
SDL_LINK_FLAGS=-lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
endif
endif

SRCFILES=$(wildcard ./$(SRCFOLDER)/*$(SOURCE_EXT))
HEADERS:=$(foreach EXT,$(HEADER_EXT),$(wildcard ./$(INCFOLDER)/*$(EXT)))
OBJECTS=$(subst $(SOURCE_EXT),.o,$(subst ./$(SRCFOLDER)/,./$(OBJFOLDER)/,$(SRCFILES)))

PERCENT=0
CURCOUNT=0

ifeq ($(OS),Windows_NT)
OBJCOUNT=$(shell echo|set /a $(words $(OBJECTS))+1)
else
OBJCOUNT=$$(($(words $(OBJECTS))+1))
endif

#DEVELOPEMENT
all: objdir $(FILE)
	@ echo [100%%] Built target $(FILE)

$(FILE): $(OBJECTS)
ifeq ($(OS),Windows_NT)
	@ $(eval PERCENT=$(shell echo|set /a $(CURCOUNT)*100/$(OBJCOUNT)))
	@ if $(PERCENT) LSS 10 (echo [  $(PERCENT)%%] [92mLinking executable $(FILE)[0m) else (echo [ $(PERCENT)%%] [92mLinking executable $(FILE)[0m)
	@ $(CC) $^ $(COMPILATION_FLAGS) $(LINKER_FLAGS) $(SDL_LINK_FLAGS) -o $(FILE) $(foreach I,./$(INCFOLDER)/,$(shell echo -I$(I)))
else
	@ printf "[%3i%%] \e[92mLinking executable %s\e[0m\n" "$$(($(CURCOUNT)*100/$(OBJCOUNT)))" "$(FILE)"
	@ $(CC) $^ $(COMPILATION_FLAGS) $(LINKER_FLAGS) $(SDL_LINK_FLAGS) -o $(FILE) $(foreach I,$(INCFOLDER)/,$(shell echo -I$(I)))
endif

./$(OBJFOLDER)/%.o: ./$(SRCFOLDER)/%$(SOURCE_EXT) ./$(INCFOLDER)/%$(HEADER_EXT)
ifeq ($(OS),Windows_NT)
	@ $(eval PERCENT=$(shell echo|set /a $(CURCOUNT)*100/$(OBJCOUNT)))
	@ if $(PERCENT) LSS 10 (echo [  $(PERCENT)%%] [32mBuilding $(CC) object $@[0m) else (echo [ $(PERCENT)%%] [32mBuilding $(CC) object $@[0m)
	@ $(CC) $< -c $(COMPILATION_FLAGS) $(SDL_COMP_FLAGS) -o $@ $(foreach I,./$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo|set /a $(CURCOUNT)+1))
else
	@ printf "[%3i%%] \e[32mBuilding %s object %s\e[0m\n" "$$(($(CURCOUNT)*100/$(OBJCOUNT)))" "$(CC)" "$@"
	@ $(CC) $< -c $(COMPILATION_FLAGS) $(SDL_COMP_FLAGS) -o $@ $(foreach I,$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo $$(($(CURCOUNT)+1))))
endif

./$(OBJFOLDER)/$(MAINFILE).o: ./$(SRCFOLDER)/$(MAINFILE)$(SOURCE_EXT) $(HEADERS)
ifeq ($(OS),Windows_NT)
	@ $(eval PERCENT=$(shell echo|set /a $(CURCOUNT)*100/$(OBJCOUNT)))
	@ if $(PERCENT) LSS 10 (echo [  $(PERCENT)%%] [32mBuilding $(CC) object $@[0m) else (echo [ $(PERCENT)%%] [32mBuilding $(CC) object $@[0m)
	@ $(CC) $< -c $(COMPILATION_FLAGS) -o $@ $(foreach I,./$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo|set /a $(CURCOUNT)+1))
else
	@ printf "[%3i%%] \e[32mBuilding %s object %s\e[0m\n" "$$(($(CURCOUNT)*100/$(OBJCOUNT)))" "$(CC)" "$@"
	@ $(CC) $< -c $(COMPILATION_FLAGS) -o $@ $(foreach I,$(INCFOLDER)/,$(shell echo -I$(I))) $(foreach d,$(DEFINES),$(shell echo -D$(d)))
	@ $(eval CURCOUNT=$(shell echo $$(($(CURCOUNT)+1))))
endif

objdir:
ifeq ($(OS),Windows_NT)
	-@ if NOT EXIST "$(OBJFOLDER)" (mkdir "$(OBJFOLDER)" >nul)
else
	@ mkdir -p "$(OBJFOLDER)"
endif

clean:
ifeq ($(OS),Windows_NT)
	-@ del /f /s /q "$(OBJFOLDER)\*.o" "$(FILE).exe" *~ >nul 2>nul
else
	@ rm -rf "$(OBJFOLDER)/*.o" "$(FILE)" *~
endif

.PHONY: all clean objdir
