all:
	gcc -shared -fPIC -o libsharedebug.so libsharedebug.c
	sudo cp ./libsharedebug.so /usr/lib/libsharedebug.so
	sudo cp ./libsharedebug.h /usr/include/libsharedebug/libsharedebug.h
	gcc -o userspace userspace.c -L. -lsharedebug
	./userspace
clean:
	rm ./userspace
	sudo rm /lib/libsharedebug.so
