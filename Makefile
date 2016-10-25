CC = gcc
MAKE = make
CFLAGS = -Wall -pedantic -s -fPIC -g
LDFLAGS = -lgmp -laqua -lcrypto
link_version = 0.1
version = $(link_version).1

debug: target = debug
debug:
	mkdir -p build/lib
	$(MAKE) -C build/lib -f ../../Makefile librandomid.so
	$(MAKE) -C cli -f Makefile $(target)

librandomid.so: randomid.o
	$(CC) -shared -Wl,-soname,librandomid.so.$(link_version) -o librandomid.so.$(link_version) randomid.o
	ln -sf librandomid.so.$(link_version) librandomid.so

%.o: ../../lib/%.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -rf build
