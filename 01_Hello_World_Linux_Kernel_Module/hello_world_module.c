#include <linux/module.h>
#include <linux/init.h>

// Meta Information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Takehiro");
MODULE_DESCRIPTION("Hello world for Linux kernel module");

// Called when the module is loaded into the kernel
static int __init ModuleInit(void) {
  printk("Hello, Linux kernel!\n");
  return 0;
}

// Called when the module is removed from the kernel
static void __exit ModuleExit(void) {
  printk("Goodbye, Linux kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
