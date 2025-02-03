all: test, test_bst, test_veb, test_correctness

CC = g++
OPT= -g -flto -Ofast
CFLAGS = $(OPT) -Wall -march=native
LIBS = -lssl -lcrypto 

test: test
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
test_bst: test-bst.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_veb: test-veb.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

test_correctness: test_correctness.cc
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f test test.o
