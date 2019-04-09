#include <linux/bpf.h>

int main() {
  return XDP_DROP;
}

char _license[] SEC("license") = "GPL";