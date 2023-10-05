#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int hello_world(struct __sk_buff *skb) {
	return 0;
}

char _license[] SEC("license") = "GPL";
