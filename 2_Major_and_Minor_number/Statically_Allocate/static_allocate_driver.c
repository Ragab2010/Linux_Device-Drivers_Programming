#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>

//creating the dev with our custom major and minor number
dev_t dev = MKDEV(50 , 0);

static int __init hello_world_init(void){
        /*
    * register_chrdev_region use to allocate static major & minor number for driver
    int register_chrdev_region(dev_t from, unsigned count, const char * name);
        unregister a range of device numbers

        Parameters
        * register_chrdev_region() - register a range of device numbers
        * @from: the first in the desired range of device numbers; must include
        *        the major number.
        * @count: the number of consecutive device numbers required
        * @name: the name of the device or driver.
        *
        * Return value is zero on success, a negative error code on failure.
    */
    register_chrdev_region(dev ,1 , "static_allocate_driver" );
    printk("Major = %d Minor = %d \n" , MAJOR(dev) , MINOR(dev));
    printk("Kernel Module Inserted Successfully...\n");
    return 0;
}

static void __exit hello_world_exit(void){
    /*
    * unregister_chrdev_region use to unallocate static major & minor number for driver
    void unregister_chrdev_region(dev_t from, unsigned count)
        unregister a range of device numbers

        Parameters
        dev_t from    : the first in the range of numbers to unregister
        unsigned count: the number of device numbers to unregister
        Description:
        This function will unregister a range of count device numbers, starting with from. The caller should normally be the one who allocated those numbers in the first place...
        * If @major == 0 this functions will dynamically allocate a major and return
        * its number.
        *
        * If @major > 0 this function will attempt to reserve a device with the given
        * major number and will return zero on success.
        *
        * Returns a -ve errno on failure.
    */
    unregister_chrdev_region(dev , 1);
    pr_info("Kernel Module Removed Successfully...\n");
}



module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR("Ragab Elsayed <ragabmx2002@gmail.com>");
MODULE_LICENSE("GPL");