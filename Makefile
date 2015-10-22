CC=clang
DEBUGFLAGS=-fPIC -O0 -g
CFLAGS=-Wall #-std=c11
LIBS=-lm -lpthread
NO_DOT = boid box files nextstep distance
OUT_BIN=boids4

all: debug

lib:
	for dot_c in $(NO_DOT); do \
		$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $$dot_c.c -o $$dot_c.o ; \
	done
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -static main.c boid.o box.o files.o nextstep.o distance.o $(LIBS) -o $(OUT_BIN)

debug:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) *.c $(LIBS) -o $(OUT_BIN)

optimize:
	$(CC) $(CFLAGS) -O3 -Wall *.c $(LIBS) -o $(OUT_BIN)
