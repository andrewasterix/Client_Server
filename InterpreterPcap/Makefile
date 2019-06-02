CC ?= gcc
CFlags ?= -O3 -std=gnu99

.PHONY: all clean

EthIp: EthIp.c 
	$(CC) $(CFlags) -c $< -o $@ -lm

all: EthIp

clean:
	rm -f EthIp