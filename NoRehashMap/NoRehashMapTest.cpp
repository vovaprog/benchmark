#include <iostream>
#include <NoRehashMap.h>

int main()
{
    typedef NoRehashMap<int, int> MapType;
    MapType m;

    for(int i=1;i<=31;++i)
    {
        std::cout << i << std::endl;

        MapType::value_type val(i, i);
        m.insert(val);
    }

    MapType::iterator iter = m.find(30);
    std::cout << iter->second << std::endl;

    /*for(int i=1;i<=100;++i)
    {
        MapType::iterator iter = m.find(i);
        assert(iter->second == i);
        std::cout << iter->second << std::endl;
    }*/

    return 0;
}
