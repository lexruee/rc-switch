CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-lwiringPi -lpthread
BINARIES=*.o

all: rc_switch.o

rc_switch.o:
	$(CC) $(CFLAGS) rc_switch.c -o rc_switch.o $(LDFLAGS)

test_sender.o: test/test_sender.c rc_switch.o
	$(CC) test/test_sender.c -o test/test_sender.o rc_switch.o $(LDFLAGS)
	
	
test_receiver.o: test/test_receiver.c rc_switch.o
	$(CC) test/test_receiver.c -o test/test_receiver.o rc_switch.o $(LDFLAGS)

test_switch: test_switch_a.o test_switch_b.o test_switch_c.o test_switch_d.o

test_switch_a.o: test/test_switch_a.c rc_switch.o
	$(CC) test/test_switch_a.c -o test/test_switch_a.o rc_switch.o $(LDFLAGS)

test_switch_b.o: test/test_switch_b.c rc_switch.o
	$(CC) test/test_switch_b.c -o test/test_switch_b.o rc_switch.o $(LDFLAGS)

test_switch_c.o: test/test_switch_c.c rc_switch.o
	$(CC) test/test_switch_c.c -o test/test_switch_c.o rc_switch.o $(LDFLAGS)
	
test_switch_d.o: test/test_switch_d.c rc_switch.o
	$(CC) test/test_switch_c.c -o test/test_switch_d.o rc_switch.o $(LDFLAGS)
	
test: test_switch test_sender.o


.PHONY: clean
clean:
	rm -f test/*.o *.o
