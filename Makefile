GXX = gcc
CFLAGS = -pedantic -g -Wall -Wvla -Werror -Wno-error=unused-variable
OFILES = hasher_main.o hasher_io.o hasher_mine.o sha-256.o

all: hasher

hasher: $(OFILES)
	$(GXX) $(CFLAGS) hasher_main.o hasher_io.o hasher_mine.o sha-256.o -o hasher

hasher_main.o: hasher_main.c hasher_io.h hasher_mine.h
	$(GXX) $(CFLAGS) hasher_main.c -c

hasher_io.o: hasher_io.c hasher_io.h
	$(GXX) $(CFLAGS) hasher_io.c -c

hasher_mine.o: hasher_mine.c hasher_mine.h sha-256.h
	$(GXX) $(CFLAGS) hasher_mine.c -c
	
sha-256.o: sha-256.c sha-256.h
	$(GXX) $(CFLAGS) sha-256.c -c

clean:
	rm hasher *.o *~ out*