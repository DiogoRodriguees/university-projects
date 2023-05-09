CC=gcc -Wall

PROGS=shm_consumer shm_producer

all: $(PROGS)

clean:
	rm -f $(PROGS)

shm_producer:	shm_posix_producer.c
	$(CC) shm_posix_producer.c -o producer -lrt

shm_consumer:	shm_posix_consumer.c
	$(CC) shm_posix_consumer.c -o consumer -lrt
