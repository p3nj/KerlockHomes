#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>
#include <errno.h>

SEC("tp/syscalls/sys_enter_bpf")
int bpf_prog_load(struct trace_event_raw_sys_enter *ctx) {
    int cmd = ctx->args[0];
    if (cmd == BPF_PROG_LOAD) {
        union bpf_attr attr;
        bpf_probe_read(&attr, sizeof(attr), (void *)ctx->args[1]);

        // You can add logic here to check the attributes of the new BPF program
        // For example, you could check attr.type, attr.map_flags, etc.

        // Check the credentials of the calling process
        struct task_struct *task = (struct task_struct *)bpf_get_current_task();
        const struct cred *cred = BPF_CORE_READ(task, cred);
        kuid_t uid = BPF_CORE_READ(cred, uid);

        if (uid.val != 0) {
            // Block non-root users from loading BPF programs
            return -EPERM;
        }
    }
    return 0;
}

char _license[] SEC("license") = "GPL";
