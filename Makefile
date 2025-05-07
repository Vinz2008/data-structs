CC = gcc

CFLAGS = -O1
CFLAGS +=  -fsanitize=address
LDFLAGS = -fsanitize=address

all: data-structs.h

HEADERS = $(wildcard include/*.h)

data-structs.h: $(HEADERS)
	cat $^ > $@


test.o: test.c $(HEADERS)
	$(CC) -c -g $(CFLAGS) test.c -o $@

test: data-structs.h test.o
	$(CC) $(LDFLAGS) -o $@ test.o

clean:
	rm -f data-structs.h test *.o