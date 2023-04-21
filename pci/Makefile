obj-m += network_card.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	insmod network_card.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
stop:
	rmmod network_card.ko