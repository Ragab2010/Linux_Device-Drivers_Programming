#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/err.h>
/*
    -any file at linux at kernel view is just Inode.
    -to make property of file as character file of relate to /connect ot character device
    -there at Inode field called (struct cdev	*i_cdev;) , we have to initialize it by (struct cdev) cdev -->(character device);
        --so we have to create and initialize (struct cdev) first. -->static struct cdev operation_cdev
        --when we create (struct cdev operation_cdev)  there is needed field called ( struct file_operations *ops) at (struct cdev operation_cdev) need to initialize it first.
            ---the struct file_operations fops field is struct of pointer to function point to operations as (open , close , read , etc).
            ---we create and initialize it  first . and go back to (struct cdev operation_cdev).
        --we create and initialize the  (struct cdev operation_cdev) by 
            1- create struct cdev (operation_cdev). 
            2- create struct file_operations (fops).
            3- initialize the  struct cdev field --> this (const struct file_operations *ops)
            by struct file_operations (fops)
            -->that happen by  cdev_init(&operation_cdev,&fops);
        4-initialize the (struct cdev *i_cdev) field that at Inode of the file --> by  struct cdev (operation_cdev)
        -->that happen by cdev_add(&operation_cdev ,dev , 1 ).
*/


//creating the dev with our custom major and minor number
dev_t dev = 0;
static struct class *dev_class;
static struct cdev operation_cdev;


static int      __init mymodule_init(void);
static void     __exit mymodule_exit(void);
static int      open_oper(struct inode *inode, struct file *file);
static int      release_oper(struct inode *inode, struct file *file);
static ssize_t  read_oper(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  write_oper(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = read_oper,
    .write      = write_oper,
    .open       = open_oper,
    .release    = release_oper,
};

/*
** This function will be called when we open the Device file
*/
static int open_oper(struct inode *inode, struct file *file)
{
        pr_info("Driver Open Function Called...!!!\n");
        return 0;
}

/*
** This function will be called when we close the Device file
*/
static int release_oper(struct inode *inode, struct file *file)
{
        pr_info("Driver Release Function Called...!!!\n");
        return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t read_oper(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Read Function Called...!!!\n");
        return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t write_oper(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Write Function Called...!!!\n");
        return len;
}


static int __init mymodule_init(void){
    /*Allocating Major number*/
    int returnValue = alloc_chrdev_region(&dev ,0 , 1 , "Operation_driver" );
    if(returnValue <0){
        printk("Cannot allocate major number for device \n");
        return -1;
    }
    printk("Major = %d Minor = %d \n" , MAJOR(dev) , MINOR(dev));

    /*Creating cdev structure*/
    /*here we initialize the struct cdev (operation_cdev) with the  struct file_operations fops .
        means:
        1- create struct cdev (operation_cdev). 
        2- create struct file_operations (fops).
        3- initialize the  struct cdev field --> this (const struct file_operations *ops)
           by struct file_operations (fops)
        -that happen by  cdev_init(&operation_cdev,&fops);
     */
    cdev_init(&operation_cdev,&fops);

    /*Adding character device to the system*/
    /*
        4-initialize the (struct cdev *i_cdev) field that at Inode of the file --> by  struct cdev (operation_cdev)
        -that happen by cdev_add(&operation_cdev ,dev , 1 )
    */
    if(cdev_add(&operation_cdev ,dev , 1 ) < 0){
        pr_err("Cannot add the device to the system\n");
        goto r_class;
    }

    /*Creating struct class*/
    dev_class=class_create(THIS_MODULE , "operation_class");
    if(IS_ERR(dev_class)){
        pr_err("Cannot create the struct class for device\n");
        goto r_class;
    }

    /*Creating device*/ 
    if(IS_ERR(device_create(dev_class , NULL , dev , NULL , "operation_device"))){
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

static void __exit mymodule_exit(void){
    device_destroy(dev_class , dev);
    class_destroy(dev_class);
    cdev_del(&operation_cdev);
    unregister_chrdev_region(dev , 1);
    pr_info("Kernel Module Removed Successfully...\n");

}


module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_AUTHOR("Ragab Elsayed <ragabmx2002@gmail.com>");
MODULE_LICENSE("GPL");