# 03 Auto device file creation and read write callbacks
Here, I create device file automatically instead of using mknod command in previous chapter.

 I also add callbacks for read and write.

## Install kernel modules in Rapberry Pi
```
sudo apt update
sudo apt install raspberrypi-kernel-headers
sudo apt upgrade
```

## Build
```
$ cd 04_gpio
$ make
```

## Load kernel modules and check it
```
$ sudo insmod gpio_driver.ko
```

## Write to device
```
$ sudo chmod 666 /dev/my_gpio_driver
```
Turn on LED
```
$ echo 1 > /dev/my_gpio_driver
```
Turn off LED
```
$ echo 0 > /dev/my_gpio_driver
```

