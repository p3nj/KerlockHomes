#ifndef KERLOCK_MODULE_H
#define KERLOCK_MODULE_H

typedef struct {
  void (*init)();
  void (*load_bpf)();
  void (*unload_bpf)();
  void (*handle_event)();
  void (*cleanup)();
} KerlockModule;

#endif // KERLOCK_MODULE_H
