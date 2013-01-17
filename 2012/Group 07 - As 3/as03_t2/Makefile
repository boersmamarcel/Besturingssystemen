CFLAGS ?= -g
CFLAGS += -Wall
CPPFLAGS += -DBE_DEBUG

all: main

main: bencode.c main.c
	cc ${CFLAGS} -o main bencode.c main.c
	
clean:
	rm -f *.o core main

.PHONY: all clean
