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

// for device and device class
static dev_t my_device_number;
static struct class *my_device_class;
static struct cdev my_device;

#define DRIVER_NAME "read_write"
#define DRIVER_CLASS "MyModuleClass"

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
  printk("Hello, Linux kernel!\n");

  // Allocate a device number
  if (alloc_chrdev_region(&my_device_number, 0, 1, DRIVER_NAME) < 0 ){
    printk("failed to allocate\n");
    return -1;
  }

  printk("read_write - registered Device numver Major: %d, Minor: %d\n",
         my_device_number >> 20,
         my_device_number&0xfffff);

  // Create a device class
  if ((my_device_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
    printk("failed to create device class\n");
    goto ClassError;
  }

  // Create a device file
  if (device_create(my_device_class, NULL, my_device_number, NULL, DRIVER_NAME) == NULL) {
    printk("failed to create device\n");
    goto FileError;
  }

  // Initialize device file
  cdev_init(&my_device, &fops);

  // Register device to kernel
  if (cdev_add(&my_device, my_device_number, 1) == -1) {
    printk("failed to register device to kernel\n");
    goto AddError;
  }

  return 0;

AddError:
  device_destroy(my_device_class, my_device_number);
FileError:
  class_destroy(my_device_class);
ClassError:
  unregister_chrdev(my_device_number, DRIVER_NAME);
  return -1;
}

// Called when the module is removed from the kernel
static void __exit ModuleExit(void) {
  cdev_del(&my_device);
  device_destroy(my_device_class, my_device_number);
  class_destroy(my_device_class);
  unregister_chrdev(my_device_number, DRIVER_NAME);

  printk("Goodbye, Linux kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
