####################################################
#
# C makefile Nature Invader
#
####################################################

PROG = natureinvader

OUT = bin

SEARCHPATH += src
vpath %.c $(SEARCHPATH)
vpath %.h $(SEARCHPATH)

DEPS += defs.h structs.h

_OBJS += background.o
_OBJS += draw.o
_OBJS += init.o input.o
_OBJS += highscores.o
_OBJS += main.o
_OBJS += sound.o stage.o
_OBJS += text.o title.o
_OBJS += util.o

OBJS = $(patsubst %,$(OUT)/%,$(_OBJS))

CC = gcc

CXXFLAGS += `sdl2-config --cflags`
CXXFLAGS += -Wall -Wempty-body -Werror -Wstrict-prototypes -Werror=maybe-uninitialized -Warray-bounds
CXXFLAGS += -g -lefence

LDFLAGS += `sdl2-config --libs` -lSDL2_mixer -lSDL2_image -lm

# define the target 'all'
all: $(PROG)

# compiling other source files.
$(OUT)/%.o: %.c %.h $(DEPS)
	@mkdir -p $(OUT)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

# linking the program
$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)	

# cleaning everything that can be automatically recreated with "make"
clean:
	$(RM) -f $(OUT) $(PROG)

# builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# builder uses this target to run your application.
run:
	./$(PROG)
