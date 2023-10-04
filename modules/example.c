#include <bpf/libbpf.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  struct bpf_object *obj;
  int prog_fd;

  // Load the BPF object file
  obj = bpf_object__open("build/example.bpf.o");
  if (libbpf_get_error(obj)) {
    fprintf(stderr, "Failed to open BPF object\n");
    return 1;
  }

  // Load BPF program
  if (bpf_object__load(obj)) {
    fprintf(stderr, "Failed to load BPF program\n");
    return 1;
  }

  prog_fd =
      bpf_program__fd(bpf_object__find_program_by_title(obj, "example_prog"));
  if (prog_fd < 0) {
    fprintf(stderr, "Failed to get BPF program file descriptor\n");
    return 1;
  }

  printf("Successfully loaded BPF program\n");

  // Unload and close BPF object
  bpf_object__unload(obj);
  bpf_object__close(obj);

  return 0;
}
