
# Creating a Device File in Linux

In Linux, we can create a device file in two ways: manually or automatically. This guide will cover both methods, focusing on manually creating a device file using the `mknod` command.

## Table of Contents

1. [Creating Device File](#creating-device-file)
   1. [Manually Creating Device File](#manually-creating-device-file)
   2. [Automatically Creating Device File](#automatically-creating-device-file)
2. [Programming Example: Creating Device File Manually](#programming-example-creating-device-file-manually)
3. [Advantages of Manual Creation](#advantages-of-manual-creation)

---

## Creating Device File

We can create a device file in two ways:

### Manually
A device file can be created manually by using the `mknod` command.

### Automatically
In an automatic method, the device file is created when the driver is loaded into the system.

---

## Manually Creating Device File

The `mknod` command is used to manually create a device file in Linux. The syntax is as follows:

```
mknod -m <permissions> <name> <device type> <major> <minor>
```

Where:
- `<name>`: The name of your device file, which should have the full path (e.g., `/dev/name`).
- `<device type>`: The type of the device file. Use:
  - `c` for **Character Device**
  - `b` for **Block Device**
- `<major>`: The major number of your device driver.
- `<minor>`: The minor number of your device driver.
- `-m <permissions>`: Optional argument that sets the permission bits for the new device file.

### Example:

```
sudo mknod -m 666 /dev/manual_create_device_file c 246 0
```

This command creates a character device file named `etx_device` with major number `246` and minor number `0`. The `-m 666` part grants read/write/execute permissions to all users.

If you don't want to set the permissions during creation, you can use the `chmod` command after creating the device file.

---

## Programming Example: Creating Device File Manually

Let's consider an example where we manually create a device file for a simple Linux driver. We will use the following code snippet, which is based on the previous tutorial.

### Source Code: `manual_create_device_file.c`

```c

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
 
dev_t dev = 0;

/*
** Module init function
*/
static int __init hello_world_init(void)
{
        /* Allocating Major number */
        if((alloc_chrdev_region(&dev, 0, 1, "manual_create_device_file")) < 0){
                pr_err("Cannot allocate major number for device\n");
                return -1;
        }
        pr_info("Kernel Module Inserted Successfully...\n");
        return 0;
}

/*
** Module exit function
*/
static void __exit hello_world_exit(void)
{
        unregister_chrdev_region(dev, 1);
        pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Simple linux driver (Manually Creating a Device file)");
MODULE_VERSION("1.1");
```

### Steps to Create the Device File:

1. **Build the driver** using a Makefile. Run `sudo make` to compile the driver.
2. **Load the driver** using `sudo insmod manual_create_device_file.ko`.
3. **Check the device file** using `ls -l /dev/`. At this point, the device file will not exist.
4. **Create the device file** manually using the `mknod` command:

   ```
   sudo mknod -m 666 /dev/manual_create_device_file c 246 0
   ```

5. **Verify the device file** has been created:

   ```
   ls -l /dev/ | grep "manual_create_device_file"
   ```

   The output should be similar to this:

   ```
   crw-rw-rw-  1  root root  246, 0  Aug 15  13:53  manual_create_device_file
   ```

   This confirms that the device file has been created with the specified major and minor numbers.

6. **Unload the driver** using the following command:

   ```
   sudo rmmod manual_create_device_file
   ```

---

## Advantages of Manual Creation

- **Flexibility**: Anyone can create a device file manually without requiring additional configuration or scripts.
- **Pre-Driver Loading**: You can create the device file before loading the actual driver, which may be useful in certain scenarios.
- **Simple**: The `mknod` command is easy to use, and it allows for quick creation of device files without needing complex automation.

---

## Conclusion

Manually creating a device file using `mknod` provides a simple, flexible way to interact with device drivers in Linux. Whether you're testing new drivers or managing device files on a production system, this method can save time and effort.

For a complete guide, check out the [source code on GitHub](https://github.com/EmbeTronicX/linux-driver).
