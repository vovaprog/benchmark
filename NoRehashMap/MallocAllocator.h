#ifndef MALLOC_ALLOCATOR_H
#define MALLOC_ALLOCATOR_H

#include <stdlib.h>

template<typename T>
class MallocAllocator
{
public:
    T* allocate(size_t count)
    {
        return reinterpret_cast<T*>(malloc(count * sizeof(T)));
    }

    void deallocate(T *p, size_t count)
    {
        free(p);
    }

    template<typename U>
    struct rebind
    {
        typedef MallocAllocator<U> other;
    };
};

#endif

