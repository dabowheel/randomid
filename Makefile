CFLAGS = -Wall -pedantic -s -g
randomid: main.o
	gcc -o randomid main.o -lgmp -laqua -lcrypto
%.o: %.c
	gcc -c $< $(CFLAGS)
clean:
	rm -f randomid main.o
