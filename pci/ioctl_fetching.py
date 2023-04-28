import fcntl

MAC_ADDRESS_SIZE = 6
DEVICE_PATH = "/dev/network_card_pci_driver"
MY_IOCTL_GET_MAC_ADDRESS = 0xed696301

if __name__ == "__main__":
    dev_file = open(DEVICE_PATH, "rb")

    mac_addr = bytearray(MAC_ADDRESS_SIZE)
    fcntl.ioctl(dev_file, MY_IOCTL_GET_MAC_ADDRESS, mac_addr)

    mac_str = ":".join(f"{x:x}" for x in mac_addr)

    print("MAC address:", mac_str)

    dev_file.close()
