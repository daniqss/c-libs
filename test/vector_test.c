#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/vector.h"
#include "../src/utils/memory.h"

#define TEST_SMALL_VECTOR 32
#define TEST_LARGE_VECTOR 1024 

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

// static void test_vector_push() {
//     Vector vector = NULL;
//     uint32_t size = 5;


//     int32_t result = vector_with_capacity(&vector, size, sizeof(int32_t));
//     assert_int_equal(result, SUCCESS);
//     assert_int_equal(vector_capacity(vector), size);
//     assert_int_equal(vector_length(vector), 0);

//     size = 10;

//     for (uint32_t i = 0; i < size; i++) {
//         result = vector_push(vector, &i);
//         assert_int_equal(result, SUCCESS);
//         assert_int_equal(vector_length(vector), i + 1);
//     }

//     for (uint32_t i = 0; i < size; i++) {
//         result = vector_pop(vector);
//         assert_int_equal(result, SUCCESS);
//         assert_int_equal(vector_length(vector), size - i - 1);
//     }

//     vector_delete(&vector);
//     assert_null(vector);
// }

// static void test_vector_at() {
//     Vector vector = NULL;
//     int64_t array[] = {
//         0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
//         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
//     };
//     int8_t *element = NULL;
//     const void *ptr;
//     int64_t result[TEST_SMALL_VECTOR];

//     assert_int_equal(vector_from(&vector, array, TEST_SMALL_VECTOR, sizeof(int64_t)), SUCCESS);
//     assert_int_equal(vector_capacity(vector), TEST_SMALL_VECTOR);
//     assert_int_equal(vector_length(vector), TEST_SMALL_VECTOR);

//     for (uint32_t i = 0; i < TEST_SMALL_VECTOR; i++) {
//         result[i] = vector_clone_at(vector, (void **)&element, i);
//         assert_int_equal(result[i], SUCCESS);
//         assert_int_equal(*element, array[i]);
//         free(element);
//     }

//     for (uint32_t i = 0; i < TEST_SMALL_VECTOR; i++) {
//         result[i] = vector_at(vector, &ptr, i);
//         assert_int_equal(result[i], SUCCESS);
//         assert_int_equal(*(int *)ptr, array[i]);
//     }

//     vector_delete(&vector);
// }

// void print_int64_t(void *element, void *args) {
//     if (args != NULL) return;
//     printf("%ld\n", *(int64_t *)element);
// }

// static void test_vector_iter() {
//     Vector vector = NULL;
//     int64_t array[] = {
//         0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
//         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
//     };

//     assert_int_equal(vector_from(&vector, array, TEST_SMALL_VECTOR, sizeof(int64_t)), SUCCESS);
//     assert_int_equal(vector_capacity(vector), TEST_SMALL_VECTOR);
//     assert_int_equal(vector_length(vector), TEST_SMALL_VECTOR);

//     // Buffer to capture the outputs
//     char vector_buffer[1024] = {0};
//     char array_buffer[1024] = {0};

//     fflush(stdout);
//     int stdout_fd = dup(1);
//     int pipe_fd[2];
//     pipe(pipe_fd);
//     dup2(pipe_fd[1], 1);
//     close(pipe_fd[1]);

//     vector_iter(vector, print_int64_t, NULL);

//     fflush(stdout);
//     read(pipe_fd[0], vector_buffer, 1024);
//     vector_buffer[1024 - 1] = '\0';
//     dup2(stdout_fd, 1);
//     close(stdout_fd);
//     close(pipe_fd[0]);


//     fflush(stdout);
//     stdout_fd = dup(1);
//     pipe(pipe_fd);
//     dup2(pipe_fd[1], 1);
//     close(pipe_fd[1]);

//     for (uint32_t j = 0; j < TEST_SMALL_VECTOR; j++)
//         print_int64_t((void *)&array[j], NULL);

//     fflush(stdout);
//     read(pipe_fd[0], array_buffer, 1024);
//     array_buffer[1024 - 1] = '\0';
//     dup2(stdout_fd, 1);
//     close(stdout_fd);
//     close(pipe_fd[0]);

//     assert_string_equal(vector_buffer, array_buffer);
//     vector_delete(&vector);
// }


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vector_new),
        // cmocka_unit_test(test_vector_push),
        // cmocka_unit_test(test_vector_at),
        // cmocka_unit_test(test_vector_iter)

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
