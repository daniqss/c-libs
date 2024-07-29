#include "include/arena.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>

#define ARENA_DEFAULT_SIZE 4294967296

typedef struct _arena_t {
    void *mem;
    uint64_t used;
    uint64_t size;
} _arena_t;

result_t arena_new(arena_t *ctx) {
    ctx->mem = mmap(NULL, ARENA_DEFAULT_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ctx->mem == MAP_FAILED) {
        return ERROR_MALLOC;
    }

    ctx->used = 0;
    ctx->size = ARENA_DEFAULT_SIZE;
    return SUCCESS;
}

result_t arena_alloc_from(arena_t *ctx, void *ptr, uint64_t size) {
    if (ctx->used + size > ctx->size) {
        return ERROR_MALLOC;
    }

    ptr = ctx->mem + ctx->used;
    ctx->used += size;
    return ptr;
}

result_t arena_delete(arena_t *ctx) {
    int32_t result = munmap(ctx->mem, ctx->size);
    if (result != 0) {
        if (errno == EINVAL) {
            return ERROR_ARGS;
        }
        return ERROR_UNKNOWN;
    }
    return SUCCESS;
}