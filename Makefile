# Compiler to use
CC = gcc
CLANG = clang

# Compiler flags
CFLAGS = -std=c11 -Iinclude

# Linker flags
LDFLAGS = -lbpf

# eBPF flags
BPF_FLAGS = -O2 -target bpf -c

# Automatically find all .c files in the 'modules' directory
MODULES = $(wildcard modules/*.c)
BPF_MODULES = $(wildcard modules/*.bpf.c)

# Create corresponding .o and .so file names in the 'build' directory
MODULES_O = $(patsubst modules/%.c, build/%.o, $(MODULES))
BPF_MODULES_O = $(patsubst modules/%.bpf.c, build/%.bpf.o, $(BPF_MODULES))

# Default target
all: build/kerlock $(MODULES_O) $(BPF_MODULES_O)

# Compile kerlock.c to build/kerlock
build/kerlock: src/kerlock.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Compile each regular module .c file to a .o file in the 'build' directory
build/%.o: modules/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Compile each eBPF module .bpf.c file to a .bpf.o file in the 'build' directory
build/%.bpf.o: modules/%.bpf.c
	$(CLANG) $(BPF_FLAGS) $< -o $@

# Clean up build files
clean:
	rm -f build/*

# Phony targets
.PHONY: all clean

