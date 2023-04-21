#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_NAME "/sharedebug"

#define EXPORT __attribute__((visibility("default")))

EXPORT int dbginit(void **ptr, size_t size);
EXPORT int dbgwrite(void *ptr, size_t size, const char *msg);
EXPORT void dbgclose(void *ptr, size_t size, int fd);

int dbginit(void **ptr, size_t size) {
    int fd;
    void *p;

    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(fd, size) < 0) {
        perror("ftruncate");
        close(fd);
        shm_unlink(SHM_NAME);
        return -1;
    }

    p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        close(fd);
        shm_unlink(SHM_NAME);
        return -1;
    }

    *ptr = p;
    return fd;
}

int dbgwrite(void *ptr, size_t size, const char *msg) {
    size_t len;

    len = strlen(msg);
    if (len >= size) {
        fprintf(stderr, "message too long for shared memory\n");
        return -1;
    }

    memcpy(ptr, msg, len);
    memset(ptr + len, '\0', size - len);
    return 0;
}

void dbgclose(void *ptr, size_t size, int fd) {
    munmap(ptr, size);
    close(fd);
    shm_unlink("/debug");
}
