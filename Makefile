
CC = gcc


CFLAGS	+= -Wall -Wextra -g $(shell pkgconf sdl2 --cflags) $(shell pkgconf criterion --cflags) $(shell pkgconf json-c --cflags)


LFLAGS = $(shell pkgconf sdl2 --libs) $(shell pkgconf criterion --libs) $(shell pkgconf json-c --libs)

OUTPUT	:= output


SRC		:= src


INCLUDE	:= include


LIB		:= documentation

ifeq ($(OS),Windows_NT)
MAIN	:= main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

OBJECTS		:= $(SOURCES:.c=.o)

DEPS		:= $(OBJECTS:.o=.d)


OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)


-include $(DEPS)


.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!
linecount:
	@echo "Counting lines in .c, .h, and .md files..."
	@find . -type f \( -name "*.c" -o -name "*.h" -o -name "*.md" \) -exec cat {} + | wc -l

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!
