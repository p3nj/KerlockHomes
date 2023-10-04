CC = gcc
CFLAGS = -std=c11 -Iinclude
LDFLAGS = -lbpf
MODULES = $(wildcard modules/*.c)
MODULES_SO = $(patsubst modules/%.c, build/%.so, $(MODULES))

all: build/kerlock $(MODULES_SO)

build/kerlock: src/kerlock.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

build/%.so: modules/%.c
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@ $(LDFLAGS)

clean:
	rm -f build/*

.PHONY: all clean

