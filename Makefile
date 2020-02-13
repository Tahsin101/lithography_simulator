# detect OS
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

all:
	gcc -g  *.c -lm -o lithosim.exe
clean:
	rm -fr lithosim
