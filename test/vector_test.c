#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/vector/vector.h"
#include "../src/utils/memory.h"

#define TEST_VECTOR_SIZE 4096

typedef struct {
    int64_t x;
    int64_t y;
    int64_t z;
} Point;

static void test_vector_new() {
    Vector vector1 = NULL;
    Vector vector2 = NULL;
    int32_t result1 = vector_new(&vector1, sizeof(int));
    int32_t result2 = vector_new(&vector2, sizeof(Point));

    assert_int_equal(result1, SUCCESS);
    assert_int_equal(vector_capacity(vector1), 1024);
    assert_int_equal(vector_length(vector1), 0);

    assert_int_equal(result2, SUCCESS);
    assert_int_equal(vector_capacity(vector2), 1024);
    assert_int_equal(vector_length(vector2), 0);

    vector_delete(&vector1);
    assert_null(vector1);
    vector_delete(&vector2);
    assert_null(vector2);
}

static void test_vector_push() {
    Vector vector1 = NULL;
    uint32_t size1 = 5;
    Vector vector2 = NULL;
    uint32_t size2 = 128;

    int32_t result1 = vector_with_capacity(&vector1, size1, sizeof(int32_t));
    assert_int_equal(result1, SUCCESS);
    assert_int_equal(vector_capacity(vector1), size1);
    assert_int_equal(vector_length(vector1), 0);
    int32_t result2 = vector_with_capacity(&vector2, size2, sizeof(Point));
    assert_int_equal(result2, SUCCESS);
    assert_int_equal(vector_capacity(vector2), size2);
    assert_int_equal(vector_length(vector2), 0);

    size1 *= 2;
    for (uint32_t i = 0; i < size1; i++) {
        result1 = vector_push(vector1, (uint8_t *)&i);
        assert_int_equal(result1, SUCCESS);
        assert_int_equal(vector_length(vector1), i + 1);
    }
    for (uint32_t i = 0; i < size1; i++) {
        result1 = vector_pop(vector1);
        assert_int_equal(result1, SUCCESS);
        assert_int_equal(vector_length(vector1), size1 - i - 1);
    }

    size2 *= 2;
    for (uint32_t i = 0; i < size2; i++) {
        Point point = {x: i, y: i + 1, z: i + 2};
        result2 = vector_push(vector2, (uint8_t *)&point);
        assert_int_equal(result2, SUCCESS);
        assert_int_equal(vector_length(vector2), i + 1);
    }
    for (uint32_t i = 0; i < size2; i++) {
        result2 = vector_pop(vector2);
        assert_int_equal(result2, SUCCESS);
        assert_int_equal(vector_length(vector2), size2 - i - 1);
    }

    vector_delete(&vector1);
    assert_null(vector1);
    vector_delete(&vector2);
    assert_null(vector2);
}

static void test_vector_at() {
    Vector vector = NULL;
    Point *array;
    void *element = NULL;
    void *ptr = NULL;
    int result[TEST_VECTOR_SIZE];

    array = malloc(TEST_VECTOR_SIZE * sizeof(Point));
    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        array[i].x = i;
        array[i].y = i * 2;
        array[i].z = i * 3;
    }

    assert_int_equal(vector_from(&vector, (uint8_t *)array, TEST_VECTOR_SIZE, sizeof(Point)), SUCCESS);
    assert_int_equal(vector_capacity(vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_length(vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_element_size(vector), sizeof(Point));

    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        result[i] = vector_clone_at(vector, (void **)&element, i);
        assert_int_equal(result[i], SUCCESS);
        Point *point_element = (Point *)element;
        assert_int_equal(point_element->x, array[i].x);
        assert_int_equal(point_element->y, array[i].y);
        assert_int_equal(point_element->z, array[i].z);
        free(element);
    }

    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        result[i] = vector_at(vector, &ptr, i);
        assert_int_equal(result[i], SUCCESS);
        Point *point_ptr = (Point *)ptr;
        assert_int_equal(point_ptr->x, array[i].x);
        assert_int_equal(point_ptr->y, array[i].y);
        assert_int_equal(point_ptr->z, array[i].z);
    }

    free(array);
    vector_delete(&vector);
    assert_null(vector);
}

void print_int64_t(void *element, void *args) {
    if (args != NULL) return;
    printf("%ld\n", *(int64_t *)element);
}

static void test_vector_iter() {
    Vector vector = NULL;
    int64_t *array;

    array = malloc(TEST_VECTOR_SIZE * sizeof(int64_t));
    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        array[i] = i;
    }

    assert_int_equal(vector_from(&vector, array, TEST_VECTOR_SIZE, sizeof(int64_t)), SUCCESS);
    assert_int_equal(vector_capacity(vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_length(vector), TEST_VECTOR_SIZE);

    // Buffer to capture the outputs
    char vector_buffer[TEST_VECTOR_SIZE * 128] = {0};
    char array_buffer[TEST_VECTOR_SIZE * 128] = {0};

    fflush(stdout);
    int stdout_fd = dup(1);
    int pipe_fd[2];
    pipe(pipe_fd);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[1]);

    vector_iter(vector, print_int64_t, NULL);

    fflush(stdout);
    read(pipe_fd[0], vector_buffer, 1024);
    vector_buffer[1024 - 1] = '\0';
    dup2(stdout_fd, 1);
    close(stdout_fd);
    close(pipe_fd[0]);


    fflush(stdout);
    stdout_fd = dup(1);
    pipe(pipe_fd);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[1]);

    for (uint32_t j = 0; j < TEST_VECTOR_SIZE; j++)
        print_int64_t((void *)&array[j], NULL);

    fflush(stdout);
    read(pipe_fd[0], array_buffer, 1024);
    array_buffer[1024 - 1] = '\0';
    dup2(stdout_fd, 1);
    close(stdout_fd);
    close(pipe_fd[0]);

    assert_string_equal(vector_buffer, array_buffer);
    free(array);
    vector_delete(&vector);
}

void multiply_point_by_factor(void *self_element, void *new_element, void *args) {
    Point *src = (Point *)self_element;
    Point *dst = (Point *)new_element;
    int factor = *(int *)args;

    dst->x = src->x * factor;
    dst->y = src->y * factor;
    dst->z = src->z * factor;
}

void max_coordinate(void *self_element, void *new_element, void *args) {
    Point *point = (Point *)self_element;
    int64_t *max = (int64_t *)new_element;

    *max = point->x;
    if (point->y > *max) *max = point->y;
    if (point->z > *max) *max = point->z;
}

static void test_vector_map() {
    Vector vector = NULL;
    Vector new_vector = NULL;
    Point *array;

    // Initialize an array of Points
    array = malloc(TEST_VECTOR_SIZE * sizeof(Point));
    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        array[i].x = i;
        array[i].y = i * 2;
        array[i].z = i * 3;
    }

    // Create a vector from the array
    assert_int_equal(vector_from(&vector, (uint8_t *)array, TEST_VECTOR_SIZE, sizeof(Point)), SUCCESS);
    assert_int_equal(vector_capacity(vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_length(vector), TEST_VECTOR_SIZE);

    // Set the factor for multiplication
    int factor = 2;

    // Apply the vector_map function to multiply points by a factor
    assert_int_equal(vector_map(vector, &new_vector, sizeof(Point), multiply_point_by_factor, &factor), SUCCESS);
    assert_int_equal(vector_capacity(new_vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_length(new_vector), TEST_VECTOR_SIZE);

    // Verify the values in the new vector
    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        void *element = NULL;
        assert_int_equal(vector_at(new_vector, &element, i), SUCCESS);
        Point *point = (Point *)element;
        
        assert_int_equal(point->x, array[i].x * factor);
        assert_int_equal(point->y, array[i].y * factor);
        assert_int_equal(point->z, array[i].z * factor);
    }

    // Case when fn is NULL
    Vector copy_vector = NULL;
    assert_int_equal(vector_map(vector, &copy_vector, sizeof(Point), NULL, NULL), SUCCESS);
    assert_int_equal(vector_capacity(copy_vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_length(copy_vector), TEST_VECTOR_SIZE);

    // Verify the values in the copied vector
    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        void *element = NULL;
        assert_int_equal(vector_at(copy_vector, &element, i), SUCCESS);
        Point *point = (Point *)element;
        
        assert_int_equal(point->x, array[i].x);
        assert_int_equal(point->y, array[i].y);
        assert_int_equal(point->z, array[i].z);
    }

    // Apply vector_map to new_vector to create a vector of int64_t
    Vector int_vector = NULL;
    assert_int_equal(vector_map(new_vector, &int_vector, sizeof(int64_t), max_coordinate, NULL), SUCCESS);
    assert_int_equal(vector_capacity(int_vector), TEST_VECTOR_SIZE);
    assert_int_equal(vector_length(int_vector), TEST_VECTOR_SIZE);

    // Verify the values in the int_vector
    for (uint32_t i = 0; i < TEST_VECTOR_SIZE; i++) {
        void *element = NULL;
        assert_int_equal(vector_at(int_vector, &element, i), SUCCESS);
        int64_t *max_value = (int64_t *)element;

        int64_t expected_max = array[i].z * factor;  // Since z is the largest component
        assert_int_equal(*max_value, expected_max);
    }

    // Clean up
    free(array);
    vector_delete(&vector);
    vector_delete(&new_vector);
    vector_delete(&copy_vector);
    vector_delete(&int_vector);
    assert_null(vector);
    assert_null(new_vector);
    assert_null(copy_vector);
    assert_null(int_vector);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vector_new),
        cmocka_unit_test(test_vector_push),
        cmocka_unit_test(test_vector_at),
        cmocka_unit_test(test_vector_iter),
        cmocka_unit_test(test_vector_map)

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
