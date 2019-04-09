#!/usr/bin/env bash
clang -O2 -Wall -target bpf -c tc_dropper.c -o tc_dropper.o;
tc filter del dev s1-eth1 ingress;
tc filter del dev s1-eth1 egress;
tc qdisc del dev s1-eth1 clsact;

tc qdisc add dev s1-eth1 clsact;
tc filter add dev s1-eth1 ingress bpf da obj tc.o sec ingress;
tc filter add dev s1-eth1 egress bpf da obj tc.o sec egress
