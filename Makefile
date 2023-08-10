# Makefile for lab10
checkers: checkers.c 
	gcc checkers.c gfx.o -lX11 -lm -no-pie -o checkers
clean:
	rm checkers

