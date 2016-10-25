CFLAGS = -Wall -pedantic -s -g
convert: main.o
	gcc -o convert main.o -lgmp -laqua -lcrypto
%.o: %.c
	gcc -c $< $(CFLAGS)
clean:
	rm -f convert main.o
