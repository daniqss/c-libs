#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include "result.h"

typedef struct _arena_t arena_t;

result_t arena_new(arena_t ctx, uint64_t size);
result_t arena_alloc_from(arena_t ctx, void *ptr, uint64_t size);
result_t arena_delete(arena_t ctx);


#endif // ARENA_H