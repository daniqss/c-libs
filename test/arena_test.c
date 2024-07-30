#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/include/arena.h"
#include "../src/include/result.h"

#define TEST_ARENA_SIZE 4096
#define ARENA_DEFAULT_SIZE (1ULL << 32) // 4 GB


static void test_arena_new() {
    arena_t arena = NULL;
    assert_int_equal(arena_new(&arena), SUCCESS);
    assert_int_equal(arena_reserved_size(arena), ARENA_DEFAULT_SIZE);
    assert_int_equal(arena_pos(arena), 0);
    assert_int_equal(arena_delete(&arena), SUCCESS);
}

static void test_arena() {
    arena_t arena = NULL;
    assert_int_equal(arena_new(&arena), SUCCESS);
    assert_int_equal(arena_reserved_size(arena), ARENA_DEFAULT_SIZE);

    for (uint64_t i = 0; i < TEST_ARENA_SIZE; i++) {
        int64_t *ptr;
        assert_int_equal(arena_push(&arena, (void **)&ptr, sizeof(int64_t) * TEST_ARENA_SIZE), SUCCESS);
        *ptr = i * i;
        assert_int_equal(*ptr, i * i);
    }
    assert_int_equal(arena_reserved_size(arena), ARENA_DEFAULT_SIZE);
    assert_int_equal(arena_pos(arena), TEST_ARENA_SIZE * sizeof(int64_t) * TEST_ARENA_SIZE);
    // assert_int_equal(arena_committed_size(arena), TEST_ARENA_SIZE * sizeof(int64_t) * TEST_ARENA_SIZE);

    assert_int_equal(arena_pop(&arena, sizeof(int64_t) * TEST_ARENA_SIZE), SUCCESS);    
    assert_int_equal(arena_pos(arena), TEST_ARENA_SIZE * sizeof(int64_t) * (TEST_ARENA_SIZE - 1));
    // assert_int_equal(arena_committed_size(arena), TEST_ARENA_SIZE * sizeof(int64_t) * TEST_ARENA_SIZE);

    assert_int_equal(arena_clear(&arena), SUCCESS);
    assert_int_equal(arena_pos(arena), 0);
    // assert_int_equal(arena_committed_size(arena), 0);

    assert_int_equal(arena_delete(&arena), SUCCESS);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_arena_new),
        cmocka_unit_test(test_arena)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}