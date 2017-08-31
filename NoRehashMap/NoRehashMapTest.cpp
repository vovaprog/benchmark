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


template<typename MapType>
bool testNoRehashMap()
{
    MapType m;

    std::vector<uint64_t> keys;

    const int itemCount = 100000;

    if(!randomVector(keys, itemCount))
    {
        std::cout << "randomVector failed!" << std::endl;
        return false;
    }

    for(int i = 0; i < itemCount; ++i)
    {
        DataCounted d;
        d.key = keys[i];

        std::pair<typename MapType::iterator, bool> p = m.insert(typename MapType::value_type(keys[i], d));

        if(!p.second)
        {
            std::cout << "insert failed!" << std::endl;
            return false;
        }

        if(m.size() != static_cast<size_t>(i + 1))
        {
            std::cout << "invalid size!" << std::endl;
            return false;
        }
    }

    for(int i = 0; i < itemCount; ++i)
    {
        if(m[keys[i]].key != keys[i])
        {
            std::cout << "find failed!" << std::endl;
            return false;
        }
    }


    for(int k = 0; k < 4; ++k)
    {
        for(int i = k % 2; i < itemCount; i += 2)
        {
            if(m.erase(keys[i]) != 1)
            {
                std::cout << "erase failed!" << " " << k << " " << i << std::endl;
                return false;
            }
        }

        if(m.size() != itemCount / 2)
        {
            std::cout << "invalid size!" << std::endl;
            return false;
        }

        for(int i = k % 2; i < itemCount; i += 2)
        {
            DataCounted d;
            d.key = keys[i];

            std::pair<typename MapType::iterator, bool> p = m.insert(typename MapType::value_type(keys[i], d));

            if(!p.second)
            {
                std::cout << "insert failed!" << std::endl;
                return false;
            }
        }

        if(m.size() != itemCount)
        {
            std::cout << "invalid size!" << std::endl;
            return false;
        }
    }


    for(int i = 1; i < itemCount; i += 2)
    {
        if(m.erase(keys[i]) != 1)
        {
            std::cout << "erase failed!" << std::endl;
            return false;
        }
    }

    if(m.size() != itemCount / 2)
    {
        std::cout << "invalid size!" << std::endl;
        return false;
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

        for(typename MapType::iterator iter = m.begin(); iter != m.end(); ++iter)
        {
            if(iter->first != iter->second.key)
            {
                std::cout << "iteration test failed!" << std::endl;
                return false;
            }
            if(keySet.count(iter->second.key) != 1)
            {
                std::cout << "iteration test failed!" << std::endl;
                return false;
            }
            if(keySet.erase(iter->second.key) != 1)
            {
                std::cout << "iteration test failed!" << std::endl;
                return false;
            }
        }

        if(keySet.size() != 0)
        {
            std::cout << "iteration test failed!" << std::endl;
            return false;
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

        for(typename MapType::value_type& v : m)
        {
            if(v.first != v.second.key)
            {
                std::cout << "iteration test failed!" << std::endl;
                return false;
            }
            if(keySet.count(v.second.key) != 1)
            {
                std::cout << "iteration test failed!" << std::endl;
                return false;
            }
            if(keySet.erase(v.second.key) != 1)
            {
                std::cout << "iteration test failed!" << std::endl;
                return false;
            }
        }

        if(keySet.size() != 0)
        {
            std::cout << "iteration test failed!" << std::endl;
            return false;
        }
    }

    return true;
}

int main()
{
#ifdef NDEBUG
    assert(0); // check that asserts don't work.
    std::cout << "asserts disabled" << std::endl;
#else
    std::cout << "asserts enabled" << std::endl;
#endif

    typedef NoRehashMap <
    uint64_t,
    DataCounted,
    boost::hash<uint64_t>, std::equal_to<uint64_t>,
    BlockStorageAllocator<std::pair<const uint64_t, uint64_t>>,
    MallocAllocator<void* >> MapType;

    if(!testNoRehashMap<MapType>())
    {
        return -1;
    }


    typedef NoRehashMap<uint64_t, DataCounted> MapTypeDefault;

    if(!testNoRehashMap<MapTypeDefault>())
    {
        return -1;
    }


    if(DataCounted::ctrCallCount != DataCounted::desCallCount)
    {
        std::cout << "constructor calls: " << DataCounted::ctrCallCount << std::endl;
        std::cout << "destructor calls: " << DataCounted::desCallCount << std::endl;
        return -1;
    }

    std::cout << "all tests passed" << std::endl;

    return 0;
}
