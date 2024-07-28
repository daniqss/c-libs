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

int32_t vector_realloc(Vector vector);
int32_t vector_alloc(Vector *vector, uint64_t capacity, uint64_t element_size);

int32_t vector_new(Vector *vector, uint64_t element_size) {
    return vector_alloc(vector, VECTOR_SIZE, element_size);
}


int32_t vector_from(Vector *vector, void *data, uint64_t length, uint64_t element_size) {
    int32_t result;

    if ((result = vector_alloc(vector, length, element_size)) != SUCCESS) return result;
    copy_memory(data, (void **)&(*vector)->data, length * element_size);


    (*vector)->length = length * element_size;

    return SUCCESS;
}

int32_t vector_with_capacity(Vector *vector, uint64_t capacity, uint64_t element_size) {
    return vector_alloc(vector, capacity, element_size);
}

int32_t vector_delete(Vector *vector) {
    free((*vector)->data);
    free(*vector);
    *vector = NULL;

    return SUCCESS;
}

int32_t vector_push(Vector vector, uint8_t *element) {
    if (vector->length + 1 >= vector->capacity)
        if (vector_realloc(vector) == ERROR_MALLOC)
            return ERROR_MALLOC;

    for (uint32_t byte = 0; byte < vector->element_size; byte++) {
        vector->data[vector->length + byte] = element[byte];
    }
    vector->length += vector->element_size;
    return SUCCESS;
}

int32_t vector_pop(Vector vector) {
    if (vector->length <= 0) return ERROR_ARGS;
    vector->length -= vector->element_size;
    return SUCCESS;
}

uint64_t vector_length(Vector vector) {
    return vector->length / vector->element_size;
}

uint64_t vector_capacity(Vector vector) {
    return vector->capacity / vector->element_size;
}

int32_t vector_at(Vector vector, void **element, uint32_t index) {
    if (vector == NULL) return ERROR_ARGS;
    index = index % vector->length;

    *element = &vector->data[index * vector->element_size];
    return SUCCESS;
}

int32_t vector_clone_at(Vector vector, void **element, uint32_t index) {
    if (vector == NULL) return ERROR_ARGS;
    index = index % vector->length;

    int32_t result = dump_memory(&vector->data[index * vector->element_size], element, vector->element_size);
    return result;
}

// int32_t vector_iter(Vector vector, void (*fn)(void *vector_element, void *args), void *args) {
//     if (vector == NULL) return ERROR_ARGS;
//     for (uint64_t i = 0; i < vector->length; i++)
//         fn(vector->data[i], args);

//     return SUCCESS;
// }

// int32_t vector_map(Vector vector, Vector *new_vector, uint64_t new_element_size, void *(*fn)(void *vector_element, void *args), void *args) {
//     void **new_array = NULL;
//     void *element = NULL;
    
//     if (vector == NULL) return ERROR_ARGS;
//     if ((new_array = malloc(vector->length * sizeof(void *))) == NULL) return ERROR_MALLOC;
//     for (uint64_t i = 0; i < vector->length; i++) {
//         // The given function should return a pointer to heap-allocated memory
//         element = fn(vector->data[i], args);
//         if (element == NULL) {
//             for (uint64_t j = 0; j < i; j++) {
//                 free(new_array[j]);
//             }
//             free(new_array);
//             return ERROR_MALLOC;
//         }

//         new_array[i] = element;
//     }
//     vector_from(new_vector, new_array, vector->length, new_element_size);

//     return SUCCESS;
// }

// Private Functions
int32_t vector_alloc(Vector *vector, uint64_t capacity, uint64_t element_size) {
    if (((*vector) = (Vector)malloc(sizeof(_Vector))) == NULL)
        return ERROR_MALLOC;

    (*vector)->length = 0;
    (*vector)->capacity = capacity * element_size;
    (*vector)->element_size = element_size;

    if (((*vector)->data = (uint8_t *)malloc((*vector)->capacity * sizeof(uint8_t))) == NULL)
        return ERROR_MALLOC;

    return SUCCESS;
}

int32_t vector_realloc(Vector vector) {
    uint8_t *aux = realloc(vector->data, (vector->capacity + VECTOR_SIZE * vector->element_size) * sizeof(uint8_t));
    
    if (aux == NULL)
        return ERROR_MALLOC;

    vector->capacity += VECTOR_SIZE;
    vector->data = aux;
    return SUCCESS;
}
