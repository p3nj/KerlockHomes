#ifndef KERLOCK_COMMON_H
#define KERLOCK_COMMON_H

struct event {
  int success;
  int pid;
  char comm[16];
};

#endif // KERLOCK_COMMON_H
