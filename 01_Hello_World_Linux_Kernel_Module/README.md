# 01 Hello World for Linux Kernel Module
## Optional
### Install Kernel modules in WSL2
```
$ git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
$ sudo apt install build-essential flex bison dwarves libssl-dev libelf-dev
$ cd WSL2-Linux-Kernel/
$ make KCONFIG_CONFIG=Microsoft/config-wsl
$ sudo make modules_install
```

## Build kernel module
```
$ cd 01_Hello_World_Linux_Kernel_Module
$ make
```

## Load module into kernel
```
$ sudo insmod hello_world_module.ko
```

## Check kernel module
```
$ lsmod | grep hello_world_module
hello_world_module     16384  0
```

## Check kernel log
```
$ dmesg | tail
...
[17484.590839] Hello, Linux kernel!
```

## Remove kernel module
```
$ sudo rmmod hello_world_module.ko
```

## Check kernel log
```
$ dmesg | tail
...
[17552.404618] Goodbye, Linux kernel!
```
