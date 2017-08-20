#include <iostream>
#include <NoRehashMap.h>

int main()
{
    typedef NoRehashMap<int, int> MapType;
    MapType m;

    for(int i = 1; i <= 10000; ++i)
    {
        std::cout << i << std::endl;

        //m.insert(MapType::value_type(i, i));
        m[i] = i;
    }

    for(int i = 1; i <= 10000; ++i)
    {
        //MapType::iterator iter = m.find(i);
        //assert(iter->second == i);
        //std::cout << iter->second << std::endl;

        assert(m[i] == i);
        std::cout << m[i] << std::endl;
    }

    return 0;
}
