CC=gcc -Wall

PROGS=b_read_fifo b_write_fifo

all: $(PROGS)

clean:
	rm -f $(PROGS)

# Para a compilação do programa faz a leitra do fifo
read_fifo:	b_read_fifo.cpp
	$(CC) b_read_fifo.cpp -o read_fifo
	
# Para a compilação do programa que escreve no fifo
write_fifo:	b_write_fifo.cpp
	$(CC) b_write_fifo.cpp -o write_fifo
