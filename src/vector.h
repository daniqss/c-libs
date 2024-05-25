#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct _Vector *Vector;

int32_t vector_new(Vector *vector, uint32_t element_size);
int32_t vector_from(Vector *vector, void *data, uint32_t size, uint32_t element_size);
int32_t vector_with_capacity(Vector *vector, uint32_t capacity, uint32_t element_size);
int32_t vector_delete(Vector *vector);
int32_t vector_push(Vector vector, void *element);
int32_t vector_pop(Vector vector);
int32_t vector_at(Vector vector, void **element, uint32_t index);

uint32_t vector_size(Vector vector);
uint32_t vector_capacity(Vector vector);
uint32_t vector_element_size(Vector vector);


#endif // VECTOR_H