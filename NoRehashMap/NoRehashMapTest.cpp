#include <iostream>

#include <NoRehashMap.h>
#include <MallocAllocator.h>
#include <BlockStorageAllocator.h>
#include <Tools.h>

struct DataCounted
{
    uint64_t key;
    uint64_t a, b, c, d;

    static size_t ctrCallCount;
    static size_t desCallCount;

    DataCounted()
    {
        ++ctrCallCount;
    }

    DataCounted(const DataCounted &d)
    {
        ++ctrCallCount;

        key = d.key;
    }

    ~DataCounted()
    {
        ++desCallCount;
    }
};

size_t DataCounted::ctrCallCount = 0;
size_t DataCounted::desCallCount = 0;


int main()
{
    typedef NoRehashMap <
    uint64_t,
    DataCounted,
    boost::hash<uint64_t>, std::equal_to<uint64_t>,
    BlockStorageAllocator<std::pair<const uint64_t, uint64_t>>,
    MallocAllocator<void* >> MapType;

    {
        MapType m;

        std::vector<uint64_t> keys;

        const int itemCount = 100000;

        if(!randomVector(keys, itemCount))
        {
            std::cout << "randomVector failed!" << std::endl;
            return -1;
        }

        for(int i = 0; i < itemCount; ++i)
        {
            DataCounted d;
            d.key = keys[i];

            std::pair<MapType::iterator, bool> p = m.insert(MapType::value_type(keys[i], d));

            if(!p.second)
            {
                std::cout << "insert failed!" << std::endl;
                return -1;
            }

            if(m.size() != static_cast<size_t>(i + 1))
            {
                std::cout << "invalid size!" << std::endl;
                return -1;
            }
        }

        for(int i = 0; i < itemCount; ++i)
        {
            if(m[keys[i]].key != keys[i])
            {
                std::cout << "find failed!" << std::endl;
                return -1;
            }
        }


        for(int k = 0; k < 4; ++k)
        {
            for(int i = k % 2; i < itemCount; i += 2)
            {
                if(m.erase(keys[i]) != 1)
                {
                    std::cout << "erase failed!" << " " << k << " " << i << std::endl;
                    return -1;
                }
            }

            if(m.size() != itemCount / 2)
            {
                std::cout << "invalid size!" << std::endl;
                return -1;
            }

            for(int i = k % 2; i < itemCount; i += 2)
            {
                DataCounted d;
                d.key = keys[i];

                std::pair<MapType::iterator, bool> p = m.insert(MapType::value_type(keys[i], d));

                if(!p.second)
                {
                    std::cout << "insert failed!" << std::endl;
                    return -1;
                }
            }

            if(m.size() != itemCount)
            {
                std::cout << "invalid size!" << std::endl;
                return -1;
            }
        }


        for(int i = 1; i < itemCount; i += 2)
        {
            if(m.erase(keys[i]) != 1)
            {
                std::cout << "erase failed!" << std::endl;
                return -1;
            }
        }

        if(m.size() != itemCount / 2)
        {
            std::cout << "invalid size!" << std::endl;
            return -1;
        }

        //========================================================================
        // iteration test for
        //========================================================================
        {
            std::set<uint64_t> keySet;

            for(int i = 0; i < itemCount; i += 2)
            {
                keySet.insert(keys[i]);
            }

            for(MapType::iterator iter = m.begin(); iter != m.end(); ++iter)
            {
                if(iter->first != iter->second.key)
                {
                    std::cout << "iteration test failed!" << std::endl;
                    return -1;
                }
                if(keySet.count(iter->second.key) != 1)
                {
                    std::cout << "iteration test failed!" << std::endl;
                    return -1;
                }
                if(keySet.erase(iter->second.key) != 1)
                {
                    std::cout << "iteration test failed!" << std::endl;
                    return -1;
                }
            }

            if(keySet.size() != 0)
            {
                std::cout << "iteration test failed!" << std::endl;
                return -1;
            }
        }

        //========================================================================
        // iteration test range based for
        //========================================================================
        {
            std::set<uint64_t> keySet;

            for(int i = 0; i < itemCount; i += 2)
            {
                keySet.insert(keys[i]);
            }

            for(MapType::value_type& v : m)
            {
                if(v.first != v.second.key)
                {
                    std::cout << "iteration test failed!" << std::endl;
                    return -1;
                }
                if(keySet.count(v.second.key) != 1)
                {
                    std::cout << "iteration test failed!" << std::endl;
                    return -1;
                }
                if(keySet.erase(v.second.key) != 1)
                {
                    std::cout << "iteration test failed!" << std::endl;
                    return -1;
                }
            }

            if(keySet.size() != 0)
            {
                std::cout << "iteration test failed!" << std::endl;
                return -1;
            }
        }
    }

    if (DataCounted::ctrCallCount != DataCounted::desCallCount)
    {
        std::cout << "constructor calls: " << DataCounted::ctrCallCount << std::endl;
        std::cout << "destructor calls: " << DataCounted::desCallCount << std::endl;
        return -1;
    }

    std::cout << "all tests passed" << std::endl;

    return 0;
}
