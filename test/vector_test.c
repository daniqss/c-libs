#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>

#include "../src/vector.h"

static void test_vector_new() {
    Vector vector = NULL;
    int32_t result = vector_new(vector, 10, sizeof(int));

    printf("\n%d\n", result); 
    assert_int_equal(result, 0);

    printf("\n%d\n", vector_capacity(vector));
    assert_int_equal(vector_capacity(vector), 10);
    assert_int_equal(vector_size(vector), 0);
    // assert_non_null(vector.data);
}

static void test_vector_push() {
    Vector vector = NULL;
    vector_new(vector, 10, sizeof(int));

    int element = 5;

    assert_int_equal(vector_push(vector, &element), 0);
    assert_int_equal(vector_size(vector), 1);
    assert_int_equal(*(int *)((vector_at(vector, 0))), element);
}

static void test_vector_pop() {
    Vector vector = NULL;
    vector_new(vector, 10, sizeof(int));

    int element = 5;
    vector_push(vector, &element);

    assert_int_equal(vector_pop(vector), 0);
    assert_int_equal(vector_size(vector), 0);
}

static void test_vector_at() {

    Vector vector = NULL;
    vector_new(vector, 10, sizeof(int));

    int element = 5;
    vector_push(vector, &element);

    int *result = (int *)vector_at(vector, 0);
    assert_non_null(result);
    assert_int_equal(*result, element);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vector_new),
        cmocka_unit_test(test_vector_push),
        cmocka_unit_test(test_vector_pop),
        cmocka_unit_test(test_vector_at),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
