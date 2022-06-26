# 03 Auto device file creation and read write callbacks
Here, I create device file automatically instead of using mknod command in previous chapter.

 I also add callbacks for read and write.

## Build
```
$ cd 03_auto_device_file_creation_and_read_write_callbacks
$ make
```

## Load kernel modules and check it
```
$ sudo insmod read_write.ko
$ dmesg | tail
...
[43090.076771] Hello, Linux kernel!
[43090.076777] read_write - registered Device numver Major: 242, Minor: 0
$ ls -al /dev/read_write
crw------- 1 root root 242, 0 Jun 26 21:27 /dev/read_write
```

## Write to device
```
$ sudo chmod 666 /dev/read_write
$ echo "Test write to my device" > /dev/read_write
```

## Read from device
```
$ head -n 1 /dev/read_write
Test write to my device
$ dmesg | tail
...
[43315.579699] dev number open was called
[43315.579724] dev number close was called
[43325.582342] dev number open was called
[43325.582416] dev number close was called
```
