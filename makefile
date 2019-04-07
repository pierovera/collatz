CFLAGS=-Wall -g -lm -lgmp

CC=gcc

all: collatz collatz_gmp

collatz: collatz.c

collatz_gmp: collatz_gmp.c
