#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <errno.h>
#include <linux/bpf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  struct bpf_object *obj;
  struct bpf_program *prog;
  struct bpf_link *link = NULL;

  // Load eBPF program from .bpf.o file
  obj = bpf_object__open("sigkill_monitor.bpf.o");
  if (libbpf_get_error(obj)) {
    fprintf(stderr, "Failed to open BPF object file\n");
    return 1;
  }

  // Load and attach the program
  prog = bpf_program__next(NULL, obj);
  if (libbpf_get_error(prog)) {
    fprintf(stderr, "Failed to load BPF program\n");
    return 1;
  }

  link = bpf_program__attach_trace(prog);
  if (libbpf_get_error(link)) {
    fprintf(stderr, "Failed to attach BPF program\n");
    return 1;
  }

  printf("Successfully loaded and attached BPF program. Monitoring SIGKILL "
         "signals...\n");

  // Keep the program running to keep the BPF program attached
  while (1) {
    sleep(10);
  }

  return 0;
}
