UNAME_S := $(shell uname -s)

AS:=			nasm
CC?=			clang
LD:=			ld
RM?=			rm -f

DEBUG?=		yes

NAME:=		wolfasm

CFLAGS+=	-I./include -march=native	$(shell sdl2-config --cflags)
ASFLAGS+=	-I./include/
LDFLAGS+=


# Add your OS flags here
ifeq ($(UNAME_S),Darwin)

SRC:=			$(shell find -E ./src -regex '.*\.(asm|c)')
CFLAGS+=	-Weverything
ASFLAGS+=	-f macho64
LDFLAGS+=	-lc -ldylib1.o -dynamic -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer	\
 					-macosx_version_min 10.12

else ifeq ($(UNAME_S),Linux)

SRC:=			$(shell find ./src -regextype posix-extended -regex ".*\.(asm|c)")
CFLAGS+=
ASFLAGS+=	-f elf64
LDFLAGS+=	-lc -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -dynamic-linker /lib64/ld-linux-x86-64.so.2

else
$(error Unsupported operating system)
endif

ifeq ($(DEBUG),yes)
CFLAGS+=	-O0 -g
ASFLAGS+=	-O0	-g
else
CFLAGS+=	-O2	-fomit-frame-pointer
ASFLAGS+=	-O2
endif

OBJ_S:=			$(SRC:%.asm=%.o)
OBJ_C:=			$(SRC:%.c=%.o)
OBJ:=				$(filter %.o, $(OBJ_C) $(OBJ_S))

$(NAME):	$(OBJ)
		$(LD) $(LDFLAGS) -o $(NAME) $(OBJ)

%.o: %.asm
		$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
		$(CC) $(CFLAGS) -c -o $@ $<

all: $(NAME)

clean:
		$(RM)	$(OBJ)
		$(RM)	$(NAME)

re:		clean all

.PHONY:	clean re all
