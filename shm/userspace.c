
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libsharedebug.h>

char *randstring(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    char *randomString = NULL;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (int n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

int main() {
    int fd;
    void *ptr;
    size_t size = 1024;
    char *msg = "Hello, world!";

    fd = dbginit(&ptr, size);
    if (fd < 0) {
        fprintf(stderr, "Failed to create shared memory\n");
        exit(1);
    }
    int counter = 0;

    while (1)
    {
        if (dbgwrite(ptr, size, msg) < 0) {
            fprintf(stderr, "Failed to write message to shared memory\n");
            exit(1);
        }
        printf("Wrote message to shared memory: %s\n", (char *)ptr);
        msg = randstring(10);
        sleep(5);
    }


    dbgclose(ptr, size, fd);
    printf("closed shared memory\n");

    return 0;
}