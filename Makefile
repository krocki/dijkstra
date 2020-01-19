.SUFFIXES:
TARGETS=dijkstra_adj dijkstra_csr

CC=gcc
LD=gcc
CFLAGS=-g -Wfatal-errors -Ofast -fomit-frame-pointer
LFLAGS=-g
HEADERS:=$(wildcard *.h) Makefile

all: $(TARGETS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.dSYM $(TARGETS)
