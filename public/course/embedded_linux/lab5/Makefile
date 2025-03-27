CC=arm-linux-gnueabihf-gcc
CFLAGS=-Wall -Wextra
TARGETS=lab5.1 lab5.2 lab5.3

all: $(TARGETS)

lab5.1: lab5.1.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

lab5.2: lab5.2.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

lab5.3: lab5.3.c
	$(CC) $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -f $(TARGETS)

.PHONY: all clean
