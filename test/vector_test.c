#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/vector.h"
#include "../src/utils/memory.h"

#define TEST_SMALL_VECTOR 16
#define TEST_LARGE_VECTOR 1024 

static void test_vector_new() {
    Vector vector = NULL;
    int32_t result = vector_new(&vector, sizeof(int));

    assert_int_equal(result, SUCCESS);
    assert_int_equal(vector_capacity(vector), 1024);
    assert_int_equal(vector_size(vector), 0);

    vector_delete(&vector);
    assert_null(vector);
}

static void test_vector_push() {
    Vector vector = NULL;
    uint32_t size = 5;


    int32_t result = vector_with_capacity(&vector, size, sizeof(int32_t));
    assert_int_equal(result, SUCCESS);
    assert_int_equal(vector_capacity(vector), size);
    assert_int_equal(vector_size(vector), 0);

    size = 10;

    for (uint32_t i = 0; i < size; i++) {
        result = vector_push(vector, &i);
        assert_int_equal(result, SUCCESS);
        assert_int_equal(vector_size(vector), i + 1);
    }

    for (uint32_t i = 0; i < size; i++) {
        result = vector_pop(vector);
        assert_int_equal(result, SUCCESS);
        assert_int_equal(vector_size(vector), size - i - 1);
    }

    vector_delete(&vector);
    assert_null(vector);
}

static void test_vector_at() {
    Vector vector = NULL;
    int64_t array[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    };
    int8_t *element = NULL;
    int64_t result[TEST_SMALL_VECTOR];

    assert_int_equal(vector_from(&vector, array, TEST_SMALL_VECTOR, sizeof(int64_t)), SUCCESS);
    assert_int_equal(vector_capacity(vector), TEST_SMALL_VECTOR);
    assert_int_equal(vector_size(vector), TEST_SMALL_VECTOR);

    for (uint32_t i = 0; i < TEST_SMALL_VECTOR; i++) {
        result[i] = vector_at(vector, (void **)&element, i);
        assert_int_equal(result[i], SUCCESS);
        assert_int_equal(*element, array[i]);
        free(element);
    }

    vector_delete(&vector);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vector_new),
        cmocka_unit_test(test_vector_push),
        cmocka_unit_test(test_vector_at),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
