#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAC_ADDRESS_SIZE 6
#define DEVICE_PATH "/dev/network_card_pci_driver"

#define MY_IOCTL_CMD _IOR('mac', 1, char*)

int main() {
    unsigned char mac_addr[MAC_ADDRESS_SIZE];

    int fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        printf("Failed to open device file.\n");
        return EXIT_FAILURE;
    }
    
    if (ioctl(fd, MY_IOCTL_CMD, mac_addr) < 0) {
        printf("Failed to perform ioctl operation.\n");
        close(fd);
        return EXIT_FAILURE;
    }
    
    printf("MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    close(fd);
    return 0;
}
