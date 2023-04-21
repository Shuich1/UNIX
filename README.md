# Homework on the discipline "Programming for UNIX"

This project contains two folders, chardev and dbus-connection-monitor, which have their own independent implementations.

## chardev

This folder contains a simple character driver implementation that includes operations such as open, close, read and write. It also includes a userspace application, userspace.c, which interacts with the character driver and writes logs to /var/log/character_driver.log. Additionally, there is a systemd service file, chardev.service, which runs the userspace application. 

The Makefile in this folder has the following options:
- `all` - Builds the kernel module, inserts it into the system, builds the userspace application, moves it and the service to the desired directory, and starts it.
- `clean` - Removes all built files.
- `stop` - Stops the service.
- `restart` - Restarts the service.

## dbus-connection-monitor

This folder consists of a shell script, internet-usage.sh, which writes logs about the connection to the Internet in the file /var/log/internet-usage.log. There is also a systemd service file, dbus-connection-monitor.service, which runs the script.

The Makefile in this folder has the following options:
- `all` - Moves all files to the correct locations and starts the service.
- `stop` - Stops the service.
- `restart` - Restarts the service.

To run each of the folders, navigate to the directory in the terminal and run the desired option from the Makefile.

## shm

This folder consists of shared memory library. If you want, you can use the provided library, libsharedebug.so, which contains functions to initialize, write to and close a shared memory block.

To use the library, you can follow these steps:

1. Compile your code and link it with the library by using the -L flag to specify the directory containing the library and the -l flag to specify the name of the library.
2. Use the functions in your code by including the header file, libdebug.h, in your source files.
3. When running your application, make sure that the shared library, libdebug.so, is accessible by either installing it in a system directory or by adding its location to the LD_LIBRARY_PATH environment variable.

To compile and link the code with the library, you can use the following command:

`gcc -o myapp myapp.c -L. -lsharedebug`

Remember to make sure that the library file, libdebug.so, is located in the same directory as your source file, or in a directory specified by the LD_LIBRARY_PATH environment variable.

You can see an example build in Makefile.

The Makefile in this folder has the following options:
- `all` - Builds library and userspace example application and running this.
- `clean` - Removes all built files.

## pci

This folder contains a Linux kernel module that implements a PCI driver for a network card. The driver is responsible for registering with the PCI subsystem, mapping the device's memory space into the kernel's address space, and exposing the device to the rest of the kernel.

The Makefile in this folder has the following options:
- `all` - Builds the kernel module, inserts it into the system.
- `clean` - Removes all built files.
- `stop` - Removes kernel module from system.