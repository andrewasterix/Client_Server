CC ?= gcc
CFlags ?= -O3 -std=gnu99

.PHONY: all clean

client: ClientSentMessage.c
	$(CC) $(CFlags) -c $< -o $@ -lm

server: ServerRecivesMessage.c
	$(CC) $(CFlags) -c $< -o $@ -lm

all: client server

clean:
	rm -f server
	rm -f client