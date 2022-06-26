# 02 Register device number
## Build
```
$ cd 02_device_number
$ make
```

## Load kernel modules and check it
```
$ sudo insmod device_number.ko
$ cat /proc/devices  | grep my_dev_number
 90 my_dev_number
$ dmesg | tail
...
[26383.752299] Hello, Linux kernel!
[26383.752306] my_dev_number - registered Device numver Major: 90, Minor: 0
```

## Create device file
```
$ sudo mknod /dev/my_device c 90 0
```
Major device number is 90 and minor device number is 0.
```
$ ls -al /dev/my_device
crw-r--r-- 1 root root 90, 0 Jun 25 21:05 /dev/my_device
```

## Open and close device file from test c program
The callback for open/close is called by open/close the device file from the c program.
```
$ gcc test.c -o test
$ ./test
succeed to open
$ dmesg | tail
...
[27801.264655] dev number open was called
[27801.264762] dev number close was called
```

## Remove kernel module
```
$ sudo rmmod device_number.ko
```