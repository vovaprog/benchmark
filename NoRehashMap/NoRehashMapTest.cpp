#include <iostream>

#include <NoRehashMap.h>
#include <MallocAllocator.h>
#include <BlockStorageAllocator.h>

int main()
{
    typedef NoRehashMap<
            int,
            int,
            boost::hash<int>, std::equal_to<int>,
            BlockStorageAllocator<std::pair<const int, int>>,
            MallocAllocator<void*>> MapType;
    MapType m;

    const int itemCount = 100000;

    for(int i = 1; i <= itemCount; ++i)
    {
        std::cout << i << std::endl;

        m.insert(MapType::value_type(i, i));
        //m[i] = i;
    }

    for(int i = 1; i <= itemCount; ++i)
    {
        //MapType::iterator iter = m.find(i);
        //assert(iter->second == i);
        //std::cout << iter->second << std::endl;

        assert(m[i] == i);
        std::cout << m[i] << std::endl;
    }

    return 0;
}
