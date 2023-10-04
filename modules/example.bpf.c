#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

SEC("filter")
int example_prog(struct __sk_buff *skb) {
  struct ethhdr *eth = bpf_hdr_pointer_cast(skb->data, 0);
  struct iphdr *ip = bpf_hdr_pointer_cast(eth + 1, 0);

  if (ip->protocol == IPPROTO_ICMP) {
    return XDP_DROP;
  }

  return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
