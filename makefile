## Files
source=$(wildcard *.c)
object=$(source:.c=.o)

#SDL2
SDL2=`sdl2-config --cflags --libs --static-libs`
CFLAGS=$(SDL2)

all:$(object)

%.o:%.c %.h

chip8:$(object)
	$(CC) $(object) -o$@ $(CFLAGS)
