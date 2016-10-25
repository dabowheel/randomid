CC = gcc
MAKE = make
CFLAGS = -Wall -pedantic -s -g
LDFLAGS = -lgmp -laqua -lcrypto

debug:
	mkdir -p ../build/cli
	$(MAKE) -C ../build/cli -f ../../cli/Makefile randomid

randomid: main.o ../lib/librandomid.so
	$(CC) -o randomid main.o ../lib/librandomid.so $(LDFLAGS)

%.o: ../../cli/%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rf ../build/cli
