CC ?= gcc
CFlags ?= -O3 -std=gnu99

.PHONY: all clean

client: Client_file_upload_server.c
	$(CC) $(CFlags) -c $< -o $@ -lm

server: Server_file_reciver_client.c
	$(CC) $(CFlags) -c $< -o $@ -lm

all: client server

clean:
	rm -f server
	rm -f client