#include "kerlock_common.h"
#include "kerlock_module.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_module(const char *module_path) {
  void *handle;
  void (*module_func)();

  handle = dlopen(module_path, RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "dlopen failed: %s\n", dlerror());
    return;
  }

  dlerror(); // Clear any existing errors

  module_func = (void (*)())dlsym(handle, "module_function");

  if (module_func) {
    module_func();
  } else {
    fprintf(stderr, "dlsym failed: %s\n", dlerror());
  }

  dlclose(handle);
}

int main() {
  struct dirent *entry;
  DIR *dir = opendir("./bin");

  if (dir == NULL) {
    perror("Unable to read directory");
    return 1;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strstr(entry->d_name, ".so")) {
      char path[256];
      snprintf(path, sizeof(path), "./bin/%s", entry->d_name);
      printf("Loading module: %s\n", path);
      load_module(path);
    }
  }

  closedir(dir);
  return 0;
}
