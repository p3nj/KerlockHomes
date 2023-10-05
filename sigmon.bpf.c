#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/ptrace.h>
#include <sys/errno.h>

SEC("tp/syscalls/sys_enter_kill")
int bpf_prog1(struct trace_event_raw_sys_enter *ctx) {
  int sig = ctx->args[1];
  if (sig == 9) { // SIGKILL
    // Block the signal by returning an error code
    return -EPERM;
  }
  return 0;
}

SEC("tp/syscalls/sys_exit_kill")
int bpf_prog2(struct trace_event_raw_sys_exit *ctx) {
  int ret = ctx->ret;
  if (ret == -EPERM) {
    // Print a message to stdout
    bpf_trace_printk("Blocked SIGKILL\n");
  }
  return 0;
}

char _license[] SEC("license") = "GPL";
