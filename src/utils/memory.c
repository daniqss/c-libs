#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/memory.h"
#include "../include/result.h"

result_t dump_memory(void *src, void **dest, uint32_t element_size) {
    if (src == NULL) return ERROR_ARGS;

    if ((*dest = malloc(element_size)) == NULL) return ERROR_MALLOC;

    for (uint32_t i = 0; i < element_size; i++)
        ((uint8_t *)(*dest))[i] = ((uint8_t *)src)[i];

    return SUCCESS;
}

result_t copy_memory(void *src, void **dest, uint32_t element_size) {
    if (src == NULL) return ERROR_ARGS;

    for (uint32_t i = 0; i < element_size; i++)
        ((uint8_t *)(*dest))[i] = ((uint8_t *)src)[i];

    return SUCCESS;
}