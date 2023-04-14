obj-m += chardev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod chardev.ko
	gcc -o chardev userspace.c
	sudo mv ./chardev /bin/chardev
	sudo cp ./chardev.service /etc/systemd/system/chardev.service
	systemctl start chardev
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
stop:
	systemctl stop chardev
	sudo rmmod chardev.ko