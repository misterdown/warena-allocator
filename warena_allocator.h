#ifndef WAREANA_ALLOCATOR_H_
#define WAREANA_ALLOCATOR_H_
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#define WAREANA_ALLOCATOR_KIBIBYTE(x__) (x__ * 1024)
#define WAREANA_ALLOCATOR_DATA_SIZE_IN_BYTES WAREANA_ALLOCATOR_KIBIBYTE(4)

// pribably faster than sbrk

typedef struct warena_allocator {
    max_align_t data[WAREANA_ALLOCATOR_DATA_SIZE_IN_BYTES];
    size_t currentIndex; // NOT A SIZE OF DATA IN BYTES
} warena_allocator;

warena_allocator warena_create() {
    warena_allocator result;
    assert((uintptr_t)result.data % alignof(max_align_t) == 0);
    return result;
}
void* warena_allocate(warena_allocator* arena, size_t bytes) {
    void* result = (void*)&arena->data[arena->currentIndex];
    arena->currentIndex += (bytes / sizeof(max_align_t)) - (bytes % sizeof(max_align_t) == 0 ? 1 : 0);
    return result;
}
void warena_destroy(warena_allocator* arena) {
    arena->currentIndex = 0;
}

#endif // ifndef WAREANA_ALLOCATOR_H_