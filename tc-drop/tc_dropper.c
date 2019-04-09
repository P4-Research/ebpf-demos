#include <linux/bpf.h>
#include <linux/pkt_cls.h>
#include <stdint.h>
#include <iproute2/bpf_elf.h>

#ifndef __section
# define __section(NAME)                  \
   __attribute__((section(NAME), used))
#endif

__section("ingress")
int tc_ingress(struct __sk_buff *skb)
{
    return TC_ACT_OK;
}

__section("egress")
int tc_egress(struct __sk_buff *skb)
{
    return TC_ACT_SHOT;
}

char __license[] __section("license") = "GPL";