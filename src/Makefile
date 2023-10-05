OUTPUT := .output
CLANG ?= clang-11
LLVM_STRIP ?= llvm-strip-11
BPFTOOL ?= $(abspath ../tools/bpftool)
LIBBPF_SRC := $(abspath ../libbpf/src)
LIBBPF_OBJ := $(abspath $(OUTPUT)/libbpf.a)
INCLUDES := -I$(OUTPUT)
CFLAGS := -g -Wall
ARCH := $(shell uname -m | sed 's/x86_64/x86/')

APPS = kerlockhomes
# MODULES = $(patsubst ../modules/%.bpf.c,$(OUTPUT)/%.bpf.o,$(wildcard ../modules/*.bpf.c))
MODULES = $(patsubst ../modules/%.bpf.c,bin/%.bpf.o,$(wildcard ../modules/*.bpf.c))


all: $(APPS) $(MODULES)

$(OUTPUT):
	mkdir -p $(OUTPUT)

$(OUTPUT)/libbpf:
	mkdir -p $(OUTPUT)/libbpf

# Build libbpf
$(LIBBPF_OBJ): $(wildcard $(LIBBPF_SRC)/*.[ch] $(LIBBPF_SRC)/Makefile) | $(OUTPUT)/libbpf
	$(call msg,LIB,$@)
	$(Q)$(MAKE) -C $(LIBBPF_SRC) BUILD_STATIC_ONLY=1		      \
		    OBJDIR=$(dir $@)/libbpf DESTDIR=$(dir $@)		      \
		    INCLUDEDIR= LIBDIR= UAPIDIR=			      \
		    install

#$(OUTPUT)/bin/%.bpf.o: ../modules/%.bpf.c $(LIBBPF_OBJ)
#	$(CLANG) -g -O2 -target bpf -D__TARGET_ARCH_$(ARCH) $(INCLUDES) -c $< -o $@
#	$(LLVM_STRIP) -g $@
#
bin/%.bpf.o: ../modules/%.bpf.c $(LIBBPF_OBJ)
	$(CLANG) -g -O2 -target bpf -D__TARGET_ARCH_$(ARCH) $(INCLUDES) -c $< -o $@
	$(LLVM_STRIP) -g $@


kerlockhomes: kerlockhomes.c $(LIBBPF_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o bin/$@ -lelf -lz

clean:
	rm -rf $(OUTPUT) bin/

.PHONY: all clean
