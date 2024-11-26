CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic

all: nsh.exe

nsh.exe: nsh.o
	$(CC) $(CFLAGS) -o $@ $^

nsh.o: nsh.h

clean:
	rm -f nsh.exe nsh.o