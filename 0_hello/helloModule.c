#include <linux/module.h>
#include <linux/init.h>



static int __init helloworld_init(void) {

    printk("myModule:-myModule Hello Kernel");
    return 0;
}

static void __exit helloworld_exit(void) {
    printk("myModule:-Goodbye, Kernel\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ragab Elsyaed");
MODULE_DESCRIPTION("Linux kernel module skeleton");


// #include <linux/module.h>
// #include <linux/init.h>

// /* Meta Information */
// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Ragab ");
// MODULE_DESCRIPTION("A hello world Psuedo device driver");

// /**
//  * @brief This function is called, when the module is loaded into the kernel
//  */
// static int __init my_init(void)
// {
// 	printk("Hello, Kernel!\n");

// 	return 0;
// }

// /**
//  * @brief This function is called, when the module is removed from the kernel
//  */
// static void __exit my_exit(void)
// {
// 	printk("Goodbye, Kernel\n");
// }

// module_init(my_init);
// module_exit(my_exit);

