#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

// Meta Information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Takehiro");
MODULE_DESCRIPTION("A gpio driver for setting a LED and get status of a push button");

// for device and device class
static dev_t my_device_number;
static struct class *my_device_class;
static struct cdev my_device;

#define DRIVER_NAME "my_gpio_driver"
#define DRIVER_CLASS "MyModuleClass"

// Read data from GPIO 17 into user buffer
static ssize_t driver_read(struct file *f, char *user_buf, size_t user_buf_size, loff_t *offset) {
  int bytes_to_copy, not_copied, delta;
  char tmp[3] = " \n";

  bytes_to_copy = min(user_buf_size, sizeof(tmp));
  tmp[0] = gpio_get_value(17) + '0';
  printk("value of the button: %c", tmp[0]);

  not_copied = copy_to_user(user_buf, tmp, bytes_to_copy);

  delta = bytes_to_copy - not_copied;

  return delta;
}

// Write data from user buffer to GPIO 4
static ssize_t driver_write(struct file *f, const char *user_buf, size_t user_buf_size, loff_t *offset) {
  int bytes_to_copy, not_copied, delta;
  char value;

  bytes_to_copy = min(user_buf_size, sizeof(value));

  not_copied = copy_from_user(&value, user_buf, bytes_to_copy);

  // set the LED
  switch(value) {
    case '0':
      gpio_set_value(4, 0); 
      break;
    case '1':
      gpio_set_value(4, 1); 
      break;
    default:
      printk("Invalid input: %c\n", value);
      break;
  }

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

  printk("gpio_driver - registered Device numver Major: %d, Minor: %d\n",
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

  // GPIO 4 init. LED are connected to GPIO 4
  if (gpio_request(4, "rpi-gpio-4")) {
    printk("failed to allocate gpio 4\n");
    goto AddError;
  }

  // set GPIO 4 direction
  if (gpio_direction_output(4, 0)) {
    printk("failed to set gpio 4 to output (default=0)\n");
    goto Gpio4Error;
  }

  // GPIO 17 init. Push button are connected to GPIO 17
  if (gpio_request(17, "rpi-gpio-17")) {
    printk("failed to allocate gpio 17\n");
    goto Gpio4Error;
  }

  // set GPIO 17 direction
  if (gpio_direction_input(17)) {
    printk("failed to set gpio 17 to input\n");
    goto Gpio17Error;
  }

  return 0;

Gpio17Error:
  gpio_free(17);
Gpio4Error:
  gpio_free(4);
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
  gpio_set_value(4, 0);
  gpio_free(17);
  gpio_free(4);
  cdev_del(&my_device);
  device_destroy(my_device_class, my_device_number);
  class_destroy(my_device_class);
  unregister_chrdev(my_device_number, DRIVER_NAME);

  printk("Goodbye, Linux kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
