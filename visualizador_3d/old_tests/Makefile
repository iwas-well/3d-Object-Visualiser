LDLIBS=-lSDL2
CC=gcc

ALL=wireframe

wireframe: wireframe.c graphics.o perspect.o objread.o
	$(CC) $(LDLIBS) -Wall -o wireframe wireframe.c graphics.o perspect.o objread.o

graphics.o:  graphics.c graphics.h datatypes.h 
	$(CC) $(LDLIBS) -Wextra -Wall -c graphics.c graphics.h datatypes.h -o graphics.o
objread.o:  objread.c objread.h datatypes.h 
	$(CC) -Wextra -Wall -c objread.c objread.h datatypes.h -o objread.o
perspect.o:  perspect.c perspect.h datatypes.h 
	$(CC) -Wextra -Wall -c perspect.c perspect.h datatypes.h -o perspect.o



clean:
	rm -f *.o *.gch

purge: clean
	rm -f wireframe
