#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#define VENDOR_ID 0x10ec
#define DRIVER_ID 0x8168

#define DRIVER_NAME "network_card_pci_driver"

#define IOCTL_GET_MAC_ADDRESS _IOR('mac', 1, char *) // 0xed696301

static struct pci_device_id network_card_pci_driver_id_table[] = {
    { PCI_DEVICE(VENDOR_ID, DRIVER_ID) },
    {0, }
};

MODULE_DEVICE_TABLE(pci, network_card_pci_driver_id_table);

static int network_card_pci_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void network_card_pci_driver_remove(struct pci_dev *pdev);

static int network_card_open(struct inode *inode, struct file *file);
static int network_card_release(struct inode *inode, struct file *file);
static long network_card_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct pci_driver network_card_pci_driver = {
    .name = DRIVER_NAME,
    .id_table = network_card_pci_driver_id_table,
    .probe = network_card_pci_driver_probe,
    .remove = network_card_pci_driver_remove
};

static struct file_operations network_card_fops = {
    .open = network_card_open,
    .release = network_card_release,
    .unlocked_ioctl = network_card_ioctl,
};

enum {
    CDEV_NOT_USED = 0,
    CDEV_EXCLUSIVE_OPEN = 1,
};

static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static int Major;
static struct class *cls;

unsigned char mac_addr[6];

static int __init network_card_pci_driver_init(void) {
    int result = pci_register_driver(&network_card_pci_driver);
    if(result < 0){
        return result;
    }

    Major = register_chrdev(0, DRIVER_NAME, &network_card_fops);
	if (Major < 0) {
		printk(KERN_ALERT "Network_card: Registering char device failed with %d\n", Major);
		return Major;
	}
	printk(KERN_INFO "Network_card: I was assigned major number %d.", Major);

    cls = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cls, NULL, MKDEV(Major, 0), NULL, DRIVER_NAME);

    return result;
}

static void __exit network_card_pci_driver_exit(void){
    device_destroy(cls, MKDEV(Major, 0));
	class_destroy(cls);
	unregister_chrdev(Major, DRIVER_NAME);

	printk(KERN_INFO "Network_card: Device finished\n");

    return pci_unregister_driver(&network_card_pci_driver);
}

static int network_card_pci_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
    u8 __iomem *mmio_base;

    int bar = 2;

    if (pci_resource_flags(pdev, bar)&IORESOURCE_MEM){
        printk(KERN_INFO "Network_card: Correct PCI resource flag.\n");
    }
    else {
        printk(KERN_ALERT "Network_card: Incorrect PCI resource flag.\n");
        return -ENODEV;
    }

    unsigned long mmio_start = pci_resource_start(pdev, bar);
    unsigned long mmio_len = pci_resource_len(pdev, bar);
            
    if ((mmio_start == 0) || (mmio_len == 0)) {
        printk(KERN_ALERT "Network_card: Failed to get pci resources: %lx - start %lu - len\n", mmio_start, mmio_len);
        return -ENOMEM;
    }

    // if (!request_mem_region(mmio_start, mmio_len, "mac_mmio")) {
    //     printk(KERN_ALERT "Network_card: Failed to request memory region\n");
    //     return -ENOMEM;
    // }

    mmio_base = ioremap(mmio_start, mmio_len);
    
    if(!mmio_base){
        printk(KERN_ALERT "Network_card: Failed to get pci resources, ioremap failed\n");
        return -EIO;
    }
    
    int i;
    for (i = 0; i < 6; i++) {
        mac_addr[i] = ioread8(mmio_base + i);
    }

    printk(KERN_INFO "Network_card: MAC address %02x:%02x:%02x:%02x:%02x:%02x\n",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    printk(KERN_INFO "Network_card: Vendor - %04x, Driver - %04x\n", VENDOR_ID, DRIVER_ID);
    printk(KERN_INFO "Network_card: Base address - %p, region len - %lu\n", mmio_base, (unsigned long)mmio_len);
    
    iounmap(mmio_base);

    return 0;
}

static int network_card_open(struct inode *inode, struct file *file) {
	if (atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) {
        printk(KERN_ALERT "Network_card: Failed to open character device\n");
		return -EBUSY;
	}

	try_module_get(THIS_MODULE);
    printk(KERN_INFO "Network_card: Character device opened\n");

	return 0;
}

static int network_card_release(struct inode *inode, struct file *file) {
	atomic_set(&already_open, CDEV_NOT_USED);
	module_put(THIS_MODULE);

    printk(KERN_INFO "Network_card: Character device closed\n");
	return 0;
}

static long network_card_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
        case IOCTL_GET_MAC_ADDRESS:
            if (copy_to_user((unsigned long __user *)arg, mac_addr, 6) != 0) {
                return -EFAULT;
            }
            break;
        default:
            return -ENOTTY;
    }

    return 0;
}


static void network_card_pci_driver_remove(struct pci_dev *pdev) {
    // Nothing to do...
}

MODULE_LICENSE("GPL");

module_init(network_card_pci_driver_init);
module_exit(network_card_pci_driver_exit);