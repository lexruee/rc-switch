CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-lwiringPi -lpthread
BINARIES=*.o

all: rc_switch.o

rc_switch.o:
	$(CC) $(CFLAGS) rc_switch.c -o rc_switch.o $(LDFLAGS)

test_sender.o: examples/test_sender.c rfpi.o
	$(CC) examples/test_sender.c -o test/test_sender.o rc_switch.o $(LDFLAGS)
	
	
test_receiver.o: examples/test_receiver.c rfpi.o
	$(CC) examples/test_receiver.c -o test/test_receiver.o rc_switch.o $(LDFLAGS)


.PHONY: clean
clean:
	rm -f test/*.o *.o
