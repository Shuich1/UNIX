#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#define VENDOR_ID 0x10ec
#define DRIVER_ID 0x8168
#define DRIVER_NAME "network_card_pci_driver"


static struct pci_device_id network_card_pci_driver_id_table[] = {
    { PCI_DEVICE(VENDOR_ID, DRIVER_ID) },
    {0, }
};

MODULE_DEVICE_TABLE(pci, network_card_pci_driver_id_table);

static int network_card_pci_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void network_card_pci_driver_remove(struct pci_dev *pdev);

static struct pci_driver network_card_pci_driver = {
    .name = DRIVER_NAME,
    .id_table = network_card_pci_driver_id_table,
    .probe = network_card_pci_driver_probe,
    .remove = network_card_pci_driver_remove
};


static int __init network_card_pci_driver_init(void) {
    return pci_register_driver(&network_card_pci_driver);
}

static void __exit network_card_pci_driver_exit(void){
    return pci_unregister_driver(&network_card_pci_driver);
}

static int network_card_pci_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
    u8 __iomem *mmio_base;
    u8 mac_addr[6];

    unsigned int mmio_start = pci_resource_start(pdev, 0);
    unsigned int mmio_len = pci_resource_len(pdev, 0);

    if ((mmio_start == 0) || (mmio_len == 0)) {
        printk(KERN_ALERT "Network_card: Failed to get pci resources\n");
        return -ENOMEM;
    }

    mmio_base = ioremap(mmio_start, mmio_len);

    if (!mmio_base) {
        printk(KERN_ALERT "Network_card: Failed to remap memory\n");
        return -EIO;
    }
    
    printk(KERN_INFO "Network_card: %d:%d\n", VENDOR_ID, DRIVER_ID);
    printk(KERN_INFO "%p - %lu\n", mmio_base, (unsigned long)mmio_len);

    // TODO MAC address fetching from pci memory
    // 7c:10:c9:24:5a:d6
    return 0;
}

static void network_card_pci_driver_remove(struct pci_dev *pdev) {
    // TODO
}

MODULE_LICENSE("GPL");

module_init(network_card_pci_driver_init);
module_exit(network_card_pci_driver_exit);