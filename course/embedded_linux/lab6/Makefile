CC=arm-linux-gnueabihf-gcc
CFLAGS=-Wall -Wextra
TARGETS=repeat lab6.2

all: $(TARGETS)

repeat: repeat.c
	$(CC) $(CFLAGS) -o $@ $<

lab6.2: lab6.2.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -f $(TARGETS)

copy: lab6.2
	scp lab6.2 beaglebone:/home/debian/

.PHONY: all clean copy
