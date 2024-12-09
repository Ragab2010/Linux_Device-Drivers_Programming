
# Automatically Creating Device File in Linux

In this section, we will discuss how to automatically create a device file in Linux using `udev`, the device manager for the Linux kernel. Udev dynamically creates and removes device nodes in the `/dev` directory as devices are added or removed from the system. By following the steps outlined below, we can create device files automatically when the driver is loaded.

## Table of Contents

1. [Automatically Creating Device File](#automatically-creating-device-file)
2. [Programming Example: Automatically Creating Device File](#programming-example-automatically-creating-device-file)
3. [Steps to Use the Code](#steps-to-use-the-code)
4. [Conclusion](#conclusion)

---

## Automatically Creating Device File

### Steps for Automatically Creating Device Files

To automatically create a device file, follow these steps:

1. **Include Required Header Files**  
   We need to include the following header files:
   - `linux/device.h`  
   - `linux/kdev_t.h`

2. **Create a `struct class`**  
   A `struct class` is necessary to register our device with the system. The class represents a group of devices and is created with the `class_create` function:

   ```c
   struct class * class_create(struct module *owner, const char *name);
   ```

   - `owner`: Pointer to the module that "owns" this struct class.
   - `name`: The name of this class (e.g., `"etx_class"`).

   This function returns a pointer to the class. The class should be destroyed after use with `class_destroy`:

   ```c
   void class_destroy(struct class *cls);
   ```

3. **Create the Device**  
   After creating the class, we need to create the device using the `device_create` function:

   ```c
   struct device *device_create(struct class *class, struct device *parent, dev_t dev, void *drvdata, const char *fmt, ...);
   ```

   - `class`: Pointer to the struct class created earlier.
   - `parent`: Pointer to the parent device (if any).
   - `dev`: The `dev_t` identifier for the device.
   - `drvdata`: Additional data for callbacks.
   - `fmt`: Name of the device.

   This function creates a device file in `/dev/` with the specified `dev_t` and registers it under the given class.

4. **Destroy the Device**  
   When done, the device can be destroyed using `device_destroy`:

   ```c
   void device_destroy(struct class *class, dev_t devt);
   ```

   This will remove the device file from the `/dev/` directory.

---

## Programming Example: Automatically Creating Device File

Here is a simple example of a Linux driver that automatically creates a device file when the driver is loaded:

### Source Code: `driver.c`

```c

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>

dev_t dev = 0;
static struct class *dev_class;

/*
** Module init function
*/
static int __init hello_world_init(void)
{
        /* Allocating Major number */
        if((alloc_chrdev_region(&dev, 0, 1, "automatic_create_devicefile_driver")) < 0){
                pr_err("Cannot allocate major number for device\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

        /* Creating struct class */
        dev_class = class_create(THIS_MODULE, "etx_class");
        if(IS_ERR(dev_class)){
            pr_err("Cannot create the struct class for device\n");
            goto r_class;
        }

        /* Creating device */
        if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "etx_device"))){
            pr_err("Cannot create the Device\n");
            goto r_device;
        }
        pr_info("Kernel Module Inserted Successfully...\n");
        return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev, 1);
        return -1;
}

/*
** Module exit function
*/
static void __exit hello_world_exit(void)
{
        device_destroy(dev_class, dev);
        class_destroy(dev_class);
        unregister_chrdev_region(dev, 1);
        pr_info("Kernel Module Removed Successfully...\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple linux driver (Automatically Creating a Device file)");
MODULE_VERSION("1.2");
```

### Explanation:
- **Major and Minor Number Allocation**: The `alloc_chrdev_region` function allocates a major and minor number for the device.
- **Class Creation**: The `class_create` function creates a `struct class` called `"etx_class"`.
- **Device Creation**: The `device_create` function registers the device with the class and creates a device file in `/dev/` called `"etx_device"`.
- **Cleanup**: The `device_destroy` and `class_destroy` functions clean up by removing the device and class when the module is removed.

---

## Steps to Use the Code

1. **Build the Driver**:  
   Use a `Makefile` to build the driver. Run:

   ```
   sudo make
   ```

   If you're using a Beaglebone or cross-compiling, use:

   ```
   sudo make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
   ```

2. **Load the Driver**:  
   Load the compiled driver with:

   ```
   sudo insmod driver.ko
   ```

3. **Check the Device File**:  
   After loading the driver, check the `/dev/` directory for the new device file:

   ```
   ls -l /dev/ | grep "etx_device"
   ```

   The output should look like:

   ```
   crw------- 1 root root 246, 0 Aug 15 13:36 etx_device
   ```

4. **Unload the Driver**:  
   To remove the driver, use:

   ```
   sudo rmmod driver
   ```

   This will automatically clean up the device file and unregister the class.

---

## Conclusion

Automatically creating device files with `udev` provides a more streamlined approach to managing devices in Linux. By using the `class_create` and `device_create` functions, the device file is automatically generated when the driver is loaded, which is particularly useful in dynamic environments where devices are frequently added or removed.

For a complete guide, check out the [source code on GitHub](https://github.com/EmbeTronicX/linux-driver).
```

This `README.md` provides a detailed explanation of automatically creating device files in Linux, along with clear instructions and a working example code.