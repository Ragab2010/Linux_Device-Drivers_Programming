#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>

//creating the dev with our custom major and minor number
dev_t dev;

static int __init hello_world_init(void){
    /*
        int alloc_chrdev_region(dev_t * dev, unsigned baseminor, unsigned count, const char * name)
        register a range of char device numbers

        Parameters:
        dev_t * dev        : output parameter for first assigned number
        unsigned baseminor :first of the requested range of minor numbers
        unsigned count     :the number of minor numbers required
        const char * name  : the name of the associated device or driver

        Description:
        Allocates a range of char device numbers. The major number will be chosen dynamically, and returned (along with the first minor number) in dev. 
        Returns zero or a negative error code.
    */
        /**
         * int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
			const char *name);
         * alloc_chrdev_region() - register a range of char device numbers
         * @dev: output parameter for first assigned number
         * @baseminor: first of the requested range of minor numbers
         * @count: the number of minor numbers required
         * @name: the name of the associated device or driver
         *
         * Allocates a range of char device numbers.  The major number will be
         * chosen dynamically, and returned (along with the first minor number)
         * in @dev.  Returns zero or a negative error code.
         */

    int returnValue = alloc_chrdev_region(&dev ,0 , 1 , "dynamic_allocate_driver" );
    if(returnValue <0){
        printk("Cannot allocate major number for device \n");
        return -1;
    }
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
    */
        /** void unregister_chrdev_region(dev_t from, unsigned count)
         *
         * unregister_chrdev_region() - unregister a range of device numbers
         * @from: the first in the range of numbers to unregister
         * @count: the number of device numbers to unregister
         *
         * This function will unregister a range of @count device numbers,
         * starting with @from.  The caller should normally be the one who
         * allocated those numbers in the first place...
         */
    unregister_chrdev_region(dev , 1);
    pr_info("Kernel Module Removed Successfully...\n");

}


module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR("Ragab Elsayed <ragabmx2002@gmail.com>");
MODULE_LICENSE("GPL");