CC ?= gcc
CFlags ?= -O3 -std=gnu99

.PHONY: all clean

EthIp.o: EthIp.c 
	$(CC) $(CFlags) -c $< -o $@ -lm

all: EthIp.o

clean: 
	rm -f *.o