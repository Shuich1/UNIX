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

    //int bars = pci_select_bars(pdev, IORESOURCE_MEM);

    if (pci_resource_flags(pdev, 0x5235E60)&IORESOURCE_MEM){
        printk(KERN_INFO "Network_card: Correct PCI resource flag.\n");
    }
    else {
        printk(KERN_ALERT "Network_card: Incorrect PCI resource flag.\n");
        return -ENODEV;
    }

    unsigned long mmio_start = pci_resource_start(pdev, 0x5235E60);
    unsigned long mmio_len = pci_resource_len(pdev, 0x5235E60);
            
    if ((mmio_start == 0) || (mmio_len == 0)) {
        printk(KERN_ALERT "Network_card: Failed to get pci resources: %lx - start %lu - len\n", mmio_start, mmio_len);
        return -ENOMEM;
    }

    if (!request_mem_region(mmio_start, mmio_len, "mac_mmio")) {
        printk(KERN_ALERT "Network_card: Failed to request memory region\n");
        return -ENOMEM;
    }

    mmio_base = ioremap(mmio_start, mmio_len);
    
    if(!mmio_base){
        printk(KERN_ALERT "Network_card: Failed to get pci resources, ioremap failed\n");
        return -EIO;
    }

    // int j;
    // for(j = 0; j < mmio_len / sizeof(u8); j++){
    //     printk(KERN_INFO "%02x\n ", mmio_base[j]);
    // }

    //TODO MAC address fetching from pci device memory...    

    printk(KERN_INFO "Network_card: %04x:%04x\n", VENDOR_ID, DRIVER_ID);
    printk(KERN_INFO "%p - %lu\n", mmio_base, (unsigned long)mmio_len);
    
    iounmap(mmio_base);

    return 0;
}

static void network_card_pci_driver_remove(struct pci_dev *pdev) {
    // TODO
}

MODULE_LICENSE("GPL");

module_init(network_card_pci_driver_init);
module_exit(network_card_pci_driver_exit);