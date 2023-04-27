#ifndef DEBUG_H
#define DEBUG_H

#include <stddef.h>

#define EXPORT __attribute__((visibility("default")))

EXPORT int dbginit(void **ptr, size_t size);
EXPORT int dbgwrite(void *ptr, size_t size, const char *msg);
EXPORT void dbgclose(void *ptr, size_t size, int fd);

#endif
