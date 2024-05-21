#include "vector.h"
#include <stdlib.h>
#include "utils/memory.h"
#include <stdio.h>

#define VECTOR_SIZE 1024

typedef struct _Vector {
    uint32_t size;
    uint32_t capacity;
    uint32_t element_size;
    void **data;
} _Vector;

int32_t vector_realloc(Vector vector);

int32_t vector_new(Vector vector, uint32_t capacity, uint32_t element_size) {
    if ((vector = (Vector)malloc(sizeof(_Vector))) == NULL)
        return ERROR_MALLOC;

    vector->size = 0; 
    vector->capacity = (capacity >= 1)? capacity : VECTOR_SIZE;
    vector->element_size = element_size;

    printf("\n%d\n", vector->capacity);

    if ((vector->data = (void **)malloc(vector->capacity * sizeof(void *))) == NULL)
        return ERROR_MALLOC;

    return SUCCESS;
}

int32_t vector_delete(Vector vector) {
    for (uint32_t i = 0; i < vector->size - 1; i++) {
        free(vector->data[i]);
    }
    free(vector->data);
    free(vector);

    return SUCCESS;
}

int32_t vector_push(Vector vector, void *element) {
    void *element_cpy = NULL;

    if (vector->size + 1 >= vector->capacity)
        if (vector_realloc(vector) == ERROR_MALLOC)
            return ERROR_MALLOC;

    int32_t result = copy_memory(element, element_cpy, vector->element_size);
    if (result != SUCCESS) return result;

    vector->data[vector->size++] = element_cpy;
    return SUCCESS;
}

int32_t vector_pop(Vector vector) {
    free(vector->data[--vector->size]);

    return SUCCESS;
}

void *vector_at(Vector vector, uint32_t index) {
    return vector->data[index];
}

uint32_t vector_size(Vector vector) {
    return vector->size;
}

uint32_t vector_capacity(Vector vector) {
    return vector->capacity;
}

// int32_t vector_at(Vector *vector, void *element,int32_t index) {
//     if (element != NULL) return ERROR_ARGS;
//     if (vector == NULL) return ERROR_ARGS;
//     return copy_memory(vector->data[index], element, vector->element_size);
// }

// Private Functions
int32_t vector_realloc(Vector vector) {
    void **aux = NULL;

    aux = realloc(vector->data, vector->capacity + VECTOR_SIZE * sizeof(void *));
    if (vector->data == NULL)
        return ERROR_MALLOC;

    vector->capacity += VECTOR_SIZE;
    vector->data = aux;
    return SUCCESS;
}


