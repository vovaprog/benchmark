#ifndef BLOCK_STORAGE_ALLOCATOR_H
#define BLOCK_STORAGE_ALLOCATOR_H

#include <BlockStorage.h>

template<typename T, int blockSize = 0x400>
class BlockStorageAllocator
{
public:
    BlockStorageAllocator() = default;

    BlockStorageAllocator(const BlockStorageAllocator &) = delete;
    BlockStorageAllocator(BlockStorageAllocator &&) = delete;
    BlockStorageAllocator& operator=(const BlockStorageAllocator &) = delete;
    BlockStorageAllocator& operator=(BlockStorageAllocator &&) = delete;


    T* allocate(size_t count)
    {
        assert(count == 1);

        return storage.allocate();
    }

    void deallocate(T *p, size_t count)
    {
        assert(count == 1);

        storage.free(p);
    }

    template<typename U>
    struct rebind
    {
        typedef BlockStorageAllocator<U, blockSize> other;
    };

    BlockStorage<T, blockSize> storage;
};

#endif

