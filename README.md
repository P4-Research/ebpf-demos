# `eBPF Demo`

This repository contains few examples of eBPF programs under XDP hook and TC hook. 
Every folder with demo has eBPF programme and bash script which compiles and attaches it to provided interface.

In order to run a demo please follow below steps:
1. Make sure that your system fulfills the requirements.
2. Create Mininet network (demos use interface created by mininet)

    `sudo python topo.py`

3. Run demo script e.g. 

    `./install_xdp.sh`





## Requirements 

1. Linux is required, with kernel version 4.5.0 or higher.
2. Install following packages (tutorial is Ubuntu 18.04 based):

    `$ sudo apt-get install -y make gcc libssl-dev bc libelf-dev libcap-dev \
        clang gcc-multilib llvm libncurses5-dev git pkg-config libmnl-dev bison flex \
        graphviz`
 
3. Install iproute2. Instructions are in https://cilium.readthedocs.io/en/v1.3/bpf/ .
    This package is needed for TC hook. Provided in iproute2 repo install script 
    does not copy all needed libraries to /usr/include. Let's do it by ourselves:
    
        sudo cp -r <path to iproute2>/iproute2/include /usr/include/iproute2
        
    Or install package from Ubuntu repositories (not tested):
        `sudo apt install iproute2`
