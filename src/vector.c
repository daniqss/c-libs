#include "vector.h"
#include <stdlib.h>
#include "utils/memory.h"
#include <stdio.h>
#include <string.h>

#define VECTOR_SIZE 1024

typedef struct _Vector {
    uint32_t size;
    uint32_t capacity;
    uint32_t element_size;
    void **data;
} _Vector;

int32_t vector_realloc(Vector vector);
int32_t vector_alloc(Vector *vector, uint32_t capacity, uint32_t element_size);

int32_t vector_new(Vector *vector, uint32_t element_size) {
    return vector_alloc(vector, VECTOR_SIZE, element_size);
}

int32_t vector_from(Vector *vector, void *data, uint32_t size, uint32_t element_size) {
    int32_t result = vector_alloc(vector, size, element_size);
    void *element = NULL;

    if (result != SUCCESS) return result;
    for (uint32_t i = 0; i < size; i++) {
        result = copy_memory(data + i * element_size, &element, element_size);
        if (result != SUCCESS) {
            for (uint32_t j = 0; j < i; j++) {
                free((*vector)->data[j]);
            }
            free((*vector)->data);
            return result;
        }
        (*vector)->data[i] = element;
        element = NULL;
    }
    (*vector)->size = size;

    return SUCCESS;
}

int32_t vector_with_capacity(Vector *vector, uint32_t capacity, uint32_t element_size) {
    return vector_alloc(vector, capacity, element_size);
}

int32_t vector_delete(Vector *vector) {
    for (uint32_t i = 0; i < (*vector)->size; i++) {
        // vector_pop(*vector);
        free((*vector)->data[i]);
    }
    free((*vector)->data);
    free(*vector);
    *vector = NULL;

    return SUCCESS;
}

int32_t vector_push(Vector vector, void *element) {
    void *element_cpy = NULL;

    if (vector->size + 1 >= vector->capacity)
        if (vector_realloc(vector) == ERROR_MALLOC)
            return ERROR_MALLOC;

    int32_t result = copy_memory(element, &element_cpy, vector->element_size);
    if (result != SUCCESS) return result;

    vector->data[vector->size++] = element_cpy;
    return SUCCESS;
}

int32_t vector_pop(Vector vector) {
    free(vector->data[--vector->size]);
    return SUCCESS;
}

uint32_t vector_length(Vector vector) {
    return vector->size;
}

uint32_t vector_capacity(Vector vector) {
    return vector->capacity;
}

int32_t vector_at(Vector vector, const void **element, uint32_t index) {
    if (vector == NULL) return ERROR_ARGS;
    index = index % vector->size;

    *element = vector->data[index];
    return SUCCESS;
}

int32_t vector_clone_at(Vector vector, void **element, uint32_t index) {
    if (vector == NULL) return ERROR_ARGS;
    index = index % vector->size;

    int32_t result = copy_memory(vector->data[index], element, vector->element_size);
    return result;
}

int32_t vector_iter(Vector vector, void (*fn)(void *)) {
    if (vector == NULL) return ERROR_ARGS;

    for (uint32_t i = 0; i < vector->size; i++) {
        
        fn(vector->data[i]);
    }

    return SUCCESS;
}

// Private Functions
int32_t vector_alloc(Vector *vector, uint32_t capacity, uint32_t element_size) {
    if (((*vector) = (Vector)malloc(sizeof(_Vector))) == NULL)
        return ERROR_MALLOC;

    (*vector)->size = 0;
    (*vector)->capacity = capacity;
    (*vector)->element_size = element_size;

    if (((*vector)->data = (void **)malloc((*vector)->capacity * sizeof(void *))) == NULL)
        return ERROR_MALLOC;

    return SUCCESS;
}

int32_t vector_realloc(Vector vector) {
    void **aux = NULL;

    aux = realloc(vector->data, (vector->capacity + VECTOR_SIZE) * sizeof(void *));
    if (aux == NULL)
        return ERROR_MALLOC;

    vector->capacity += VECTOR_SIZE;
    vector->data = aux;
    return SUCCESS;
}


