#ifndef BLOCK_STORAGE_ALLOCATOR_H
#define BLOCK_STORAGE_ALLOCATOR_H

#include <BlockStorage.h>

template<typename T, int blockSize = 0x400>
class BlockStorageAllocator
{
public:
    T* allocate(size_t count)
    {
        assert(count == 1);

        return storage.allocate();
    }

    void deallocate(T *p, size_t count)
    {
        assert(count == 1);

        free(p);
    }

    template<typename U>
    struct rebind
    {
        typedef MallocAllocator<U> other;
    };

    BlockStorage<T, blockSize> storage;
};

#endif

