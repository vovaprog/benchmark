#include <iostream>
#include <NoRehashMap.h>

int main()
//bool noRehashMapTest()
{
    typedef NoRehashMap<int, int> MapType;
    MapType m;

    MapType::value_type val(3, 4);
    m.insert(val);
    
    MapType::iterator iter = m.find(3);

    std::cout << iter->second << std::endl;

    //return true;
    return 0;
}
