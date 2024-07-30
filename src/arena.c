#include "include/arena.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ARENA_DEFAULT_SIZE (1ULL << 32) // 4 GB

typedef struct _arena_t {
    void *mem;
    uint64_t pos;
    uint64_t reserved_size;
    uint64_t committed_size;
} _arena_t;

result_t arena_new(arena_t *ctx) {
    *ctx = malloc(sizeof(_arena_t));

    // PROT_NONE flag allocates a contiguous virtual memory region with no permissions granted.
    // Is used to implement guards or magic pointers
    // Here we are using it to map multiple independent mappings as a virtually contiguous mapping
    // When more memory is needed, we use mprotect to change the permissions of the mapping
    (*ctx)->mem = mmap(NULL, ARENA_DEFAULT_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if ((*ctx)->mem == MAP_FAILED) {
        return ERROR_MALLOC;
    }

    (*ctx)->pos = 0;
    (*ctx)->reserved_size = ARENA_DEFAULT_SIZE;
    (*ctx)->committed_size = 0;
    return SUCCESS;
}

result_t arena_push(arena_t *ctx, void **ptr, uint64_t size) {
    if ((*ctx)->pos + size > (*ctx)->reserved_size) {
        return ERROR_MALLOC;
    }

    //
    uint64_t new_committed_size = (*ctx)->pos + size;
    if (new_committed_size > (*ctx)->committed_size) {

        // sysconf is a POSIX function that returns system configuration information at runtime
        // _SC_PAGESIZE is a macro constant that make sysconf return the size of a page in bytes
        uint64_t page_size = sysconf(_SC_PAGESIZE);
        uint64_t commit_size = ((new_committed_size + page_size - 1) / page_size) * page_size;
        
        // mprotect changes the access protections for the calling process's memory pages
        // containing any part of the address range in the interval [addr, addr+len-1]
        if (mprotect((*ctx)->mem, commit_size, PROT_READ | PROT_WRITE) != 0) {
            return ERROR_MALLOC;
        }
        (*ctx)->committed_size = commit_size;
    }

    *ptr = (*ctx)->mem + (*ctx)->pos;
    (*ctx)->pos += size;
    return SUCCESS;
}

result_t arena_pop(arena_t *ctx, uint64_t size) {
    if (size > (*ctx)->pos) {
        return ERROR_ARGS;
    }
    (*ctx)->pos -= size;
    return SUCCESS;
}

result_t arena_clear(arena_t *ctx) {
    (*ctx)->pos = 0;
    return SUCCESS;
}

uint64_t arena_pos(arena_t ctx) {
    return ctx->pos;
}

uint64_t arena_reserved_size(arena_t ctx) {
    return ctx->reserved_size;
}

uint64_t arena_committed_size(arena_t ctx) {
    return ctx->committed_size;
}

result_t arena_delete(arena_t *ctx) {
    int32_t result = munmap((*ctx)->mem, (*ctx)->reserved_size);
    if (result != 0) {
        if (errno == EINVAL) {
            return ERROR_ARGS;
        }
        return ERROR_UNKNOWN;
    }
    return SUCCESS;
}