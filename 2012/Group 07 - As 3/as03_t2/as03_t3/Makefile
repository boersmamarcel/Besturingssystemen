# Makefile for TestFS server
PROG=   testfs
SRCS=   testfs.c main.c bencode.c

DPADD+= ${LIBVTREEFS} ${LIBSYS}
LDADD+= -lvtreefs -lsys 

CFLAGS+= -D_NETBSD_SOURCE=1 
CFLAGS+= -Wall
CFLAGS+= -g

MAN=

BINDIR?= /sbin

.include <bsd.prog.mk>