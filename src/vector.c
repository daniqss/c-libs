#include "vector.h"
#include <stdlib.h>
#include "utils/memory.h"
#include <stdio.h>
#include <string.h>

#define VECTOR_SIZE 1024

typedef struct _Vector {
    uint64_t length;
    uint64_t capacity;
    uint64_t element_size;
    uint8_t *data;
} _Vector;

int32_t vector_realloc(Vector self);
int32_t vector_alloc(Vector *self, uint64_t capacity, uint64_t element_size);

int32_t vector_new(Vector *self, uint64_t element_size) {
    return vector_alloc(self, VECTOR_SIZE, element_size);
}


int32_t vector_from(Vector *self, void *data, uint64_t length, uint64_t element_size) {
    int32_t result;

    if ((result = vector_alloc(self, length, element_size)) != SUCCESS) return result;
    copy_memory(data, (void **)&(*self)->data, length * element_size);


    (*self)->length = length * element_size;

    return SUCCESS;
}

int32_t vector_with_capacity(Vector *self, uint64_t capacity, uint64_t element_size) {
    return vector_alloc(self, capacity, element_size);
}

int32_t vector_delete(Vector *self) {
    free((*self)->data);
    free(*self);
    *self = NULL;

    return SUCCESS;
}

int32_t vector_push(Vector self, uint8_t *element) {
    if (self->length + 1 >= self->capacity)
        if (vector_realloc(self) == ERROR_MALLOC)
            return ERROR_MALLOC;

    for (uint32_t byte = 0; byte < self->element_size; byte++) {
        self->data[self->length + byte] = element[byte];
    }
    self->length += self->element_size;
    return SUCCESS;
}

int32_t vector_pop(Vector self) {
    if (self->length <= 0) return ERROR_ARGS;
    self->length -= self->element_size;
    return SUCCESS;
}

uint64_t vector_length(Vector self) {
    return self->length / self->element_size;
}

uint64_t vector_capacity(Vector self) {
    return self->capacity / self->element_size;
}

int32_t vector_at(Vector self, void **element, uint32_t index) {
    if (self == NULL) return ERROR_ARGS;
    index = index % self->length;

    *element = &self->data[index * self->element_size];
    return SUCCESS;
}

int32_t vector_clone_at(Vector self, void **element, uint32_t index) {
    if (self == NULL) return ERROR_ARGS;
    index = index % self->length;

    int32_t result = dump_memory(&self->data[index * self->element_size], element, self->element_size);
    return result;
}

int32_t vector_iter(Vector self, void (*fn)(void *vector_element, void *args), void *args) {
    if (self == NULL) return ERROR_ARGS;
    for (uint64_t i = 0; i < self->length; i += self->element_size)
        fn(&self->data[i], args);

    return SUCCESS;
}

// int32_t vector_map(Vector self, Vector *new_vector, uint64_t new_element_size, void *(*fn)(void *vector_element, void *args), void *args) {
//     uint8_t *new_array = NULL;
//     void *element = NULL;
    
//     if (self == NULL) return ERROR_ARGS;
//     if ((new_array = malloc(self->length * sizeof(void *))) == NULL) return ERROR_MALLOC;

//     for (uint64_t i = 0; i < self->length; i += self->element_size) {
//         // The given function should return a pointer to heap-allocated memory
//         element = fn(&self->data[i], args);
//         if (element == NULL) {
//             free(new_array);
//             return ERROR_MALLOC;
//         }

//         copy_memory(element, (void **)&new_array[i], new_element_size);
//     }
//     vector_from(new_vector, new_array, self->length, new_element_size);

//     return SUCCESS;
// }

// Private Functions
int32_t vector_alloc(Vector *self, uint64_t capacity, uint64_t element_size) {
    if (((*self) = (Vector)malloc(sizeof(_Vector))) == NULL)
        return ERROR_MALLOC;

    (*self)->length = 0;
    (*self)->capacity = capacity * element_size;
    (*self)->element_size = element_size;

    if (((*self)->data = (uint8_t *)malloc((*self)->capacity * sizeof(uint8_t))) == NULL)
        return ERROR_MALLOC;

    return SUCCESS;
}

int32_t vector_realloc(Vector self) {
    uint8_t *aux = realloc(self->data, (self->capacity + VECTOR_SIZE * self->element_size) * sizeof(uint8_t));
    
    if (aux == NULL)
        return ERROR_MALLOC;

    self->capacity += VECTOR_SIZE;
    self->data = aux;
    return SUCCESS;
}
