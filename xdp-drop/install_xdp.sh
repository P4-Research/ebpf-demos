#!/usr/bin/env bash
clang -O2 -target bpf -c xdp_dropper.c -o xdp_dropper.o
sudo ip link set dev s1-eth1 xdp off
sudo ip link set dev s1-eth1 xdp obj xdp_dropper.o sec .text