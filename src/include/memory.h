#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

int32_t dump_memory(void *src, void **dest, uint32_t element_size);
int32_t copy_memory(void *src, void **dest, uint32_t element_size);

#endif // MEMORY_H