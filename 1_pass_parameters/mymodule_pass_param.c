#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/*
Module Parameters Macros:
module_param(variableName, variableNameType, permissions);    --->used to initialize the arguments
    -ex: module_param(valueETX, int, S_IWUSR|S_IRUSR);

module_param_array(variableArray, variableArrayType, permissions);  -->used to send the array as an argument to the Linux device driver

module_param_cb()
-->used to register the callback. Whenever the argument (parameter) got changed, this callback function will be called. I think you don’t understand. Let me explain it properly.



There are several types of permissions:


S_IWUSR
S_IRUSR
S_IXUSR
S_IRGRP
S_IWGRP
S_IXGRP
In this S_I is a common header.
R = read ,W =write ,X= Execute.
USR =user ,GRP =Group
Using OR ‘|’ (or operation) we can set multiple permissions at a time.


*/

int argValue = 0;
module_param(argValue, int,S_IWUSR|S_IRUSR); 

#define DEVICE_NAME "mymodule_device"
#define BUF_LEN 80

static char message[BUF_LEN];
static int device_open = 0;
static int majorNumber; // Declare globally

static int mydevice_open(struct inode *inode, struct file *file) {
    if (device_open) return -EBUSY;
    device_open++;
    printk("mydevice_open\n");
    return 0;
}

static int mydevice_release(struct inode *inode, struct file *file) {
    device_open--;
    printk("mydevice_release\n");
    return 0;
}

static ssize_t mydevice_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
    // Implement read functionality
    printk("mydevice_read\n");
    return 0; // Return number of bytes read
}

static ssize_t mydevice_write(struct file *file, const char __user *buffer, size_t length, loff_t *offset) {
    // Implement write functionality
    printk("mydevice_write\n");
    return length; // Return number of bytes written
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = mydevice_open,
    .release = mydevice_release,
    .read = mydevice_read,
    .write = mydevice_write,
};

static int __init helloworld_init(void) {
	int major;
    majorNumber = argValue; // Use the global variable
    printk("Hello, Kernel!\n");
    major = register_chrdev(majorNumber, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }
    printk(KERN_INFO "mydevice registered with major number %d\n", major);
    return 0;
}

static void __exit helloworld_exit(void) {
    unregister_chrdev(majorNumber, DEVICE_NAME); // Use majorNumber
    printk(KERN_INFO "mydevice unregistered\n");
    printk("Goodbye, Kernel\n");
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

