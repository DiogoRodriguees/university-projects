# compilação do professor
CC=gcc
CFLAGS=-I.
DEPS = matriz.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

matriz: matriz.o exec02.o
	$(CC) -o main matriz.o exec02.o -lm

clean: 
	rm *.o main