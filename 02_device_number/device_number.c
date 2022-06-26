#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

// Meta Information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Takehiro");
MODULE_DESCRIPTION("Register a device number and implement a callback function for open and close");

// Called when the device file is opened
static int driver_open(struct inode *device_file, struct file *instance) {
  printk("dev number open was called\n");
  return 0;
}

// Called when the device file is closed
static int driver_close(struct inode *device_file, struct file *instance) {
  printk("dev number close was called\n");
  return 0;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release =driver_close
};

// Update device number if required.
// Check available device number by command below:
//   $ cat /proc/devices 
#define MY_MAJOR_NUMBER 90

// Called when the module is loaded into the kernel
static int __init ModuleInit(void) {
  int ret;

  printk("Hello, Linux kernel!\n");

  // register device number
  ret = register_chrdev(MY_MAJOR_NUMBER, "my_dev_number", &fops);
  if (ret == 0) {
    printk("my_dev_number - registered Device numver Major: %d, Minor: %d\n",
           MY_MAJOR_NUMBER, 0);
  } else if (ret > 0) { // The major number is already in use.
                        // It registers same major number and incremented minor number.
    printk("my_dev_number - registered Device numver Major: %d, Minor: %d\n",
           ret>>20, ret&0xfffff); // ret contains Major device number in high-order 12 bit
                                  // and Minor device number in lower 20 bit
  } else {
    printk("Could not register device number\n");
    return -1;
  }

  return 0;
}

// Called when the module is removed from the kernel
static void __exit ModuleExit(void) {
  unregister_chrdev(MY_MAJOR_NUMBER, "my_dev_number");
  printk("Goodbye, Linux kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
