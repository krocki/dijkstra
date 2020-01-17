.SUFFIXES:
TARGETS=dijkstra

CC=gcc
LD=gcc
CFLAGS=-g -Wfatal-errors -O0
LFLAGS=-g
HEADERS:=$(wildcard *.h) Makefile

all: $(TARGETS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.dSYM $(TARGETS)
