
# Linux Device Drivers Programming

This repository contains a series of practical examples for programming Linux device drivers. Each directory represents a specific concept or technique used in writing device drivers for Linux, ranging from basic kernel modules to interacting with real hardware devices.

## Table of Contents

1. [Hello World Module](#1-hello-world-module)
2. [Passing Parameters to Kernel Module](#2-passing-parameters-to-kernel-module)
3. [Major and Minor Numbers](#3-major-and-minor-numbers)
4. [Device File Creation](#4-device-file-creation)
5. [File Operations in Device Drivers](#5-file-operations-in-device-drivers)
6. [Real Device Drivers](#6-real-device-drivers)

---

## 1. Hello World Module

- **Location**: `0_hello/helloModule.c`
- **Description**: A simple "Hello, World!" Linux kernel module. This example demonstrates the basic structure of a kernel module, including loading and unloading of the module.
- **Files**:
    - `helloModule.c`: The kernel module source code.
    - `Makefile`: A makefile for building the module.

## 2. Passing Parameters to Kernel Module

- **Location**: `1_pass_parameters/mymodule_pass_param.c`
- **Description**: This example shows how to pass parameters to a kernel module using the command line arguments when loading the module.
- **Files**:
    - `mymodule_pass_param.c`: The source code for the kernel module.
    - `Makefile`: A makefile for building the module.

## 3. Major and Minor Numbers

- **Location**: `2_Major_and_Minor_number/`
- **Description**: This example explains how to statically and dynamically allocate major and minor numbers for device files.
  
  - **Dynamically Allocate**: Demonstrates how to dynamically allocate a major number and register a device.
    - `dynamic_allocate_driver.c`: Source code for dynamically allocated device driver.
    - `Makefile`: Makefile for building the dynamic driver.

  - **Statically Allocate**: Demonstrates how to statically allocate a major number for device registration.
    - `static_allocate_driver.c`: Source code for statically allocated device driver.
    - `Makefile`: Makefile for building the static driver.

## 4. Device File Creation

- **Location**: `3_Device_File_Creation/`
- **Description**: This example demonstrates how to create device files automatically or manually.

  - **Automatic Method**: Shows how to automatically create a device file during module initialization.
    - `automatic_create_devicefile_driver.c`: Source code for the driver that automatically creates a device file.
    - `Makefile`: Makefile for building the automatic driver.
    - `ReadMe.md`: Explanation and details for automatic creation of device files.

  - **Manual Method**: Shows how to manually create and delete device files using `mknod`.
    - `manual_create_device_file.c`: Source code for manually creating and deleting device files.
    - `Makefile`: Makefile for building the manual driver.
    - `ReadMe.md`: Explanation and details for manual creation of device files.

## 5. File Operations in Device Drivers

- **Location**: `4_File_Operations/`
- **Description**: This example introduces file operations (`open`, `read`, `write`, etc.) for character device drivers.
- **Files**:
    - `operation_driver.c`: Source code demonstrating file operations for a device.
    - `Makefile`: Makefile for building the driver.
    - `ReadMe.md`: Detailed explanation of the various file operations and their usage.

## 6. Real Device Drivers

- **Location**: `5_Real_device_driver/`
- **Description**: This directory contains an example of a real device driver for actual hardware. It also includes a user-space application to interact with the driver.
- **Files**:
    - `real_drive.c`: Source code for the real device driver.
    - `real_drive2.c`: Additional implementation of the real device driver.
    - `test_app.c`: A user-space application to interact with the real device driver.
    - `Makefile`: Makefile for building the real device driver.
    - `ReadMe.md`: Explanation and details for the real device driver example.

---

## Getting Started

### Prerequisites

- A Linux system with kernel headers installed.
- `make` and `gcc` or other necessary tools for compiling kernel modules.

### Building and Running

1. Navigate to the relevant directory (e.g., `0_hello`, `1_pass_parameters`, etc.).
2. Run `make` to compile the kernel module.
3. Insert the module into the kernel using `sudo insmod <module_name>.ko`.
4. To remove the module, use `sudo rmmod <module_name>`.

### Example:

For the "Hello World" example:
```bash
cd 0_hello
make
sudo insmod helloModule.ko
dmesg | tail
sudo rmmod helloModule
```

---

## Contributing

Feel free to fork the repository, create issues, or submit pull requests. Contributions are always welcome.

---

## License

This repository is licensed under the MIT License. See [LICENSE](LICENSE) for more details.
