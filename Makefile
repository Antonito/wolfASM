AS:=			nasm
CC?=			clang
LD:=			ld
RM?=			rm -f

NAME:=		wolfasm

SRC:=			$(shell find -E . -regex '.*\.(asm|c)')

CFLAGS+=	-I./include -Weverything -Wno-nonportable-system-include-path -O0 -g
ASFLAGS+=	-f macho64 -I./include/ -O0
LDFLAGS+=	-lc -ldylib1.o -dynamic -lSDL2 -lSDL2_ttf -lSDL2_Image -lSDL2_Mixer -macosx_version_min 10.12

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
