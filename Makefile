AS:=			nasm
LD:=			ld
RM?=			rm -f

NAME:=		wolfasm

SRC:=			$(shell find -E . -regex '.*\.(asm)')

ASFLAGS+=	-f macho64 -I./include/ -g -O0
LDFLAGS+=	-lc -ldylib1.o -dynamic -lSDL2

OBJ:=			$(SRC:%.asm=%.o)

$(NAME):	$(OBJ)
	$(LD) $(LDFLAGS) -o $(NAME) $(OBJ)

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

all: $(NAME)

clean:
		$(RM)	$(OBJ)

re:		clean all

.PHONY:	clean re all
