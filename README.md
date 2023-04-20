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
