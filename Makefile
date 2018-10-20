# Makefile

CC=gcc

CPPFLAGS=-MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS=-lm


all: neur

neur: neur.o parser.o
parser.o: parser.h
neur.o: neur.c

clean:
	${RM} *.o
	${RM} *.d
	${RM} neur

# END
