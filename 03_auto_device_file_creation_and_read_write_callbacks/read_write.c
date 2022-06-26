#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// Meta Information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Takehiro");
MODULE_DESCRIPTION("Register a device number and implement a callback functions");

// Buffer for data
static char buffer[255];
static size_t buffer_ptr;

// Read data from the buffer into user buffer
static ssize_t driver_read(struct file *f, char *user_buf, size_t user_buf_size, loff_t *offset) {
  int bytes_to_copy, not_copied, delta;

  bytes_to_copy = min(user_buf_size, buffer_ptr);

  not_copied = copy_to_user(user_buf, buffer, bytes_to_copy);

  delta = bytes_to_copy - not_copied;

  return delta;
}

// Write data from user buffer to the buffer
static ssize_t driver_write(struct file *f, const char *user_buf, size_t user_buf_size, loff_t *offset) {
  int bytes_to_copy, not_copied, delta;

  bytes_to_copy = min(user_buf_size, sizeof(buffer));

  not_copied = copy_from_user(buffer, user_buf, bytes_to_copy);
  buffer_ptr = bytes_to_copy;

  delta = bytes_to_copy - not_copied;

  return delta;
}

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
  .release =driver_close,
  .read = driver_read,
  .write = driver_write
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
