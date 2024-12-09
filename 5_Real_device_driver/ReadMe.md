
# Using Dynamic Allocation(kmalloc ,kfree , copy_from_user , copy_to_user )

## Introduction
We already know that in Linux ueverything is a File. So in this tutorial, we are going to develop two applications:

1. **User Space application** (User program)
2. **Kernel Space program** (Driver)

The user program will communicate with the kernel space program using the device file. Let's get started!

## Kernel Space Program (Device Driver)
We already know about major and minor numbers, device files, and file operations of device drivers. If you don’t know, please visit our previous tutorials. Now, we are going to discuss more file operations in the device driver. Basically, there are four functions in the device driver:

1. **Open driver**
2. **Write Driver**
3. **Read Driver**
4. **Close Driver**

Now, let's go through these functions one by one. But first, let me explain the concept of this driver.

### Concept
Using this driver, we can send strings or data to the kernel device driver using the `write` function. It will store that string in the kernel space. Then, when we read the device file, it will send the data written by the `write` function to the userspace application.

### Functions used in this driver

- **kmalloc()**
- **kfree()**
- **copy_from_user()**
- **copy_to_user()**

#### kmalloc()
We will see the memory allocation methods available in the kernel in our next tutorials. But for now, we will only use `kmalloc()` in this tutorial.

The `kmalloc` function is used to allocate memory in the kernel space. This is like the `malloc()` function in userspace. The function is fast (unless it blocks) and doesn’t clear the memory it obtains. The allocated region still holds its previous content. The allocated region is also contiguous in physical memory.

```c
#include <linux/slab.h>
void *kmalloc(size_t size, gfp_t flags);
```

**Arguments:**
- `size` – How many bytes of memory are required.
- `flags` – The type of memory to allocate.

The `flags` argument may be one of:

- **GFP_USER** – Allocate memory on behalf of the user. May sleep.
- **GFP_KERNEL** – Allocate normal kernel RAM. May sleep.
- **GFP_ATOMIC** – Allocation will not sleep. May use emergency pools. For example, use this inside an interrupt handler.
- **GFP_HIGHUSER** – Allocate pages from high memory.
- **GFP_NOIO** – Do not do any I/O at all while trying to get memory.
- **GFP_NOFS** – Do not make any FS calls while trying to get memory.
- **GFP_NOWAIT** – Allocation will not sleep.
- **__GFP_THISNODE** – Allocate node-local memory only.
- **GFP_DMA** – Allocation is suitable for DMA. Should only be used for kmalloc caches. Otherwise, use a slab created with `SLAB_DMA`.

Additionally, you can set different flags by OR’ing in one or more of the following:

- **__GFP_COLD** – Request cache-cold pages instead of trying to return cache-warm pages.
- **__GFP_HIGH** – This allocation has high priority and may use emergency pools.
- **__GFP_NOFAIL** – Indicate that this allocation is in no way allowed to fail (use with caution).
- **__GFP_NORETRY** – If memory is not immediately available, then give up at once.
- **__GFP_NOWARN** – If allocation fails, don’t issue any warnings.
- **__GFP_REPEAT** – If allocation fails initially, try once more before failing.

There are other flags available as well, but these are not intended for general use and are not documented here. For a full list of potential flags, refer to `linux/gfp.h`.

#### kfree()
This is like the `free()` function in userspace. It is used to free the previously allocated memory.

```c
void kfree(const void *objp);
```

**Arguments:**
- `objp` – Pointer returned by `kmalloc`.

#### copy_from_user()
This function is used to copy a block of data from user space to kernel space.

```c
unsigned long copy_from_user(void *to, const void __user *from, unsigned long n);
```

**Arguments:**
- `to` – Destination address in the kernel space.
- `from` – Source address in the user space.
- `n` – Number of bytes to copy.

Returns the number of bytes that could not be copied. On success, this will be zero.

#### copy_to_user()
This function is used to copy a block of data from kernel space to user space.

```c
unsigned long copy_to_user(const void __user *to, const void *from, unsigned long n);
```

**Arguments:**
- `to` – Destination address in the user space.
- `from` – Source address in the kernel space.
- `n` – Number of bytes to copy.

Returns the number of bytes that could not be copied. On success, this will be zero.

## File Operations in the Driver

### Open
This function is called first when we are opening the device file. In this function, I will allocate memory using `kmalloc`. In the userspace application, you can use the `open()` system call to open the device file.

```c
static int etx_open(struct inode *inode, struct file *file)
{
    /* Creating physical memory */
    if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        return -1;
    }
    printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}
```

### Write
When writing data to the device file, it will call this write function. Here, I will copy the data from user space to kernel space using the `copy_from_user()` function. In the userspace application, you can use the `write()` system call to write data to the device file.

```c
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    copy_from_user(kernel_buffer, buf, len);
    printk(KERN_INFO "Data Write : Done!\n");
    return len;
}
```

### Read
When we read the device file, it will call this function. In this function, I use `copy_to_user()`. This function is used to copy the data to the userspace application. In the userspace application, you can use the `read()` system call to read the data from the device file.

```c
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    copy_to_user(buf, kernel_buffer, mem_size);
    printk(KERN_INFO "Data Read : Done!\n");
    return mem_size;
}
```

### Close
When we close the device file, this function is called. Here, I will free the memory that was allocated by `kmalloc` using `kfree()`. In the userspace application, you can use the `close()` system call to close the device file.

```c
static int etx_release(struct inode *inode, struct file *file)
{
    kfree(kernel_buffer);
    printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}
```
