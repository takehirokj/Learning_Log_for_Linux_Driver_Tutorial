# 04 GPIO driver
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

