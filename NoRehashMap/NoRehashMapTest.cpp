#include <iostream>
#include <NoRehashMap.h>

int main()
//bool noRehashMapTest()
{
    typedef NoRehashMap<int, int> MapType;
    MapType m;

    for(int i=0;i<100;++i)
    {
        std::cout << i << std::endl;

        MapType::value_type val(i, i);
        m.insert(val);
    }
    
    MapType::iterator iter = m.find(3);

    std::cout << iter->second << std::endl;

    //return true;
    return 0;
}
