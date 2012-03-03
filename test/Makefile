# Makefile for Test

OUT = ./test

CFLAGS = -Wall -O2

INCZ = -I../include -I.

SRCS = main.cpp
LIBS = -lsquirrel -lsqstdlib

t32:
	g++ $(CFLAGS) $(INCZ) -o $(OUT) $(SRCS) $(LIBS)
	-$(RM) *.o
