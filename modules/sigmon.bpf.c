// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ptrace.h>

SEC("tracepoint/syscalls/sys_enter_kill")
int bpf_prog1(struct trace_event_raw_sys_enter *ctx) {
  int sig = ctx->args[1];
  if (sig == 9) { // 9 == SIGKILL
                  // Log or take action
                  // Could use bpf_printk or other BPF helper functions here
  }
  return 0;
}

char _license[] SEC("license") = "GPL";
