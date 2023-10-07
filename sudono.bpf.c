#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include "bpf_helpers.h"

// Function to compare the file path with "/etc/sudoers"
static __always_inline bool is_sudoers_file(const char *filepath) {
    const char target[] = "/etc/sudoers";
    #pragma unroll
    for (int i = 0; i < sizeof(target); i++) {
        if (filepath[i] != target[i]) {
            return false;
        }
    }
    return true;
}

SEC("tp/syscalls/sys_enter_openat")
int handle_openat_enter(struct __sk_buff *skb) {
    // Buffer to hold the file path
    char filepath[256] = {};

    // Read the file path argument from the syscall
    int ret = bpf_probe_read_user(&filepath, sizeof(filepath), (void *)PT_REGS_PARM2(ctx));
    if (ret < 0) {
        return 0; // Exit early if read fails
    }

    // Check if the file being accessed is /etc/sudoers
    if (is_sudoers_file(filepath)) {
        // Block the syscall
        return -EPERM;
    }

    return 0;
}

char _license[] SEC("license") = "GPL";
