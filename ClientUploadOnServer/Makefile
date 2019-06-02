CC ?= gcc
CFlags ?= -O3 -std=gnu99

.PHONY: all clean

client.o: Client_file_upload_server.c
	$(CC) $(CFlags) -c $< -o $@ -lm

server.o: Server_file_reciver_client.c
	$(CC) $(CFlags) -c $< -o $@ -lm

all: client.o server.o

clean: 
	rm -f *.o