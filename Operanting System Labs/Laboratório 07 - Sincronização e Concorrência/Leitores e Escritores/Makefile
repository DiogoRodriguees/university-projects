CC=gcc -Wall

PROGS=prod_cons

all: $(PROGS)

clean:
	rm -f $(PROGS)

prod_cons: main.c
	$(CC) main.c -o prod_cons -lpthread
