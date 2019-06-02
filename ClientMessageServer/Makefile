CC ?= gcc
CFlags ?= -O3 -std=gnu99

.PHONY: all clean

client.o: ClientSentMessage.c
	$(CC) $(CFlags) -c $< -o $@ -lm

server.o: ServerRecivesMessage.c
	$(CC) $(CFlags) -c $< -o $@ -lm

all: client.o server.o

clean:
	rm -f *.o