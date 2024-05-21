#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define SUCCESS 0
#define ERROR_MALLOC (-1)
#define ERROR_ARGS (-2)

int32_t copy_memory(void *src, void *dest, uint32_t element_size);

#endif // UTILS_H