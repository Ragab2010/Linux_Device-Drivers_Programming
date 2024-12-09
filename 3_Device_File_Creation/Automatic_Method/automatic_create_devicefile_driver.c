#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

/*
    1. Reserve a major and a range of minors with alloc_chrdev_region().
    2. Create a class for your devices with class_create(), visible in /sys/class/.
    3. Set up a struct file_operation (to be given to cdev_init), and for each
    device you need to create, call cdev_init() and cdev_add() to register the
    device.
    4. Then, create a device_create() for each device, with a proper name. It will
    result in your device being created in the /dev directory:
*/

/*
struct device * device_create(struct class * class, struct device * parent, dev_t devt, void * drvdata, const char * fmt, ...)
creates a device and registers it with sysfs

Parameters:

struct class * class  :pointer to the struct class that this device should be registered to
struct device * parent:pointer to the parent struct device of this new device, if any
dev_t devt            :the dev_t for the char device to be added
void * drvdata        :the data to be added to the device for callbacks
const char * fmt      :string for the device’s name
...
variable arguments
Description:
This function can be used by char device classes. A struct device will be created in sysfs, registered to the specified class.

A “dev” file will be created, showing the dev_t for the device, if the dev_t is not 0,0. If a pointer to a parent struct device is passed in, the newly created struct device will be a child of that device in sysfs. The pointer to the struct device will be returned from the call. Any further sysfs files that might be required can be created using this pointer.

Returns struct device pointer on success, or ERR_PTR() on error.

Note

the struct class passed to this function must have previously been created with a call to class_create().
*/

//creating the dev with our custom major and minor number
dev_t dev = 0;
static struct class *dev_class;

static int __init hello_world_init(void){
    //1  create major & minor number for device driver
    int returnValue = alloc_chrdev_region(&dev ,0 , 1 , "automatic_create_devicefile_driver" );
    if(returnValue <0){
        printk("Cannot allocate major number for device \n");
        return -1;
    }
    printk("Major = %d Minor = %d \n" , MAJOR(dev) , MINOR(dev));
    //2 Create a class for your devices with class_create(), visible in /sys/class/.
    dev_class=class_create(THIS_MODULE , "automatic_class");
    if(IS_ERR(dev_class)){
        pr_err("Cannot create the struct class for device\n");
        goto r_class;
    }
    //3 Set up a struct file_operation (to be given to cdev_init), 
    //and for each  device you need to create, call cdev_init() and cdev_add() to register the device.
    //struct device * device_create(struct class * class, struct device * parent, dev_t devt, void * drvdata, const char * fmt, ...)
    if(IS_ERR(device_create(dev_class , NULL , dev , NULL , "automatic_device"))){
        pr_err("Cannot create the Device\n");
        goto r_device;
    }
    printk("Kernel Module Inserted Successfully...\n");

    return 0;
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev , 1);
    return -1;
}

static void __exit hello_world_exit(void){
    device_destroy(dev_class , dev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev , 1);
    pr_info("Kernel Module Removed Successfully...\n");

}


module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR("Ragab Elsayed <ragabmx2002@gmail.com>");
MODULE_LICENSE("GPL");