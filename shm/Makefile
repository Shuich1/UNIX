all:
	gcc -shared -fPIC -o libsharedebug.so libsharedebug.c
	sudo mv ./libsharedebug.so /lib/libsharedebug.so
	gcc -o userspace userspace.c -L. -lsharedebug
	./userspace
clean:
	rm ./userspace
	sudo rm /lib/libsharedebug.so
