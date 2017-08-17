AS:=			nasm
CC?=			clang
LD:=			ld
RM?=			rm -f

DEBUG?=		yes

NAME:=		wolfasm

SRC:=			$(shell find -E ./src -regex '.*\.(asm|c)')

CFLAGS+=	-I./include -Weverything -Wno-nonportable-system-include-path				\
 					-march=native
ASFLAGS+=	-f macho64 -I./include/
LDFLAGS+=	-lc -ldylib1.o -dynamic -lSDL2 -lSDL2_ttf -lSDL2_Image -lSDL2_Mixer	\
					-macosx_version_min 10.12

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
