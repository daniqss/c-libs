#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include "result.h"

typedef struct _Vector *Vector;

result_t vector_new(Vector *self, uint64_t element_size);
result_t vector_from(Vector *self, void *data, uint64_t size, uint64_t element_size);
result_t vector_with_capacity(Vector *self, uint64_t capacity, uint64_t element_size);
result_t vector_delete(Vector *self);
result_t vector_push(Vector self, uint8_t *element);
result_t vector_pop(Vector self);
result_t vector_at(Vector self, void **element, uint32_t index);
result_t vector_clone_at(Vector self, void **element, uint32_t index);
result_t vector_iter(Vector self, void (*fn)(void *vector_element, void *args), void *args);
result_t vector_map(
    Vector self,
    Vector *new,
    uint64_t new_element_size,
    void (*fn)(void *self_element, void *new_element, void *args),
    void *args
);
uint64_t vector_length(Vector self);
uint64_t vector_capacity(Vector self);
uint64_t vector_element_size(Vector self);


#endif // VECTOR_H