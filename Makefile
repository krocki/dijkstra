.SUFFIXES:
TARGETS=dijkstra

CC=gcc
LD=gcc
CFLAGS=-gdwarf -Wfatal-errors -O0
LFLAGS=-gdwarf
HEADERS:=$(wildcard *.h) Makefile

all: $(TARGETS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.dSYM $(TARGETS)
