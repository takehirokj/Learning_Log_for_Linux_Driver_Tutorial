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
$ sudo insmod device_number.ko
$ cat /proc/devices  | grep my_dev_number
 90 my_dev_number
```

## Create device file
TODO: delete this after implementing auto device file creation.
```
$ sudo mknod /dev/my_device c 90 0
```

## Write to device
```
$ sudo chmod 666 /dev/my_device
$ echo "Test write to my device" > /dev/my_device
```

## Read from device
```
$ head -n 1 /dev/my_device
Test write to my device
```
