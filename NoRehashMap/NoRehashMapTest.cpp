#include <NoRehashMap.h>
#include <MallocAllocator.h>
#include <BlockStorageAllocator.h>
#include <Tools.h>

#include <iostream>
#include <stdlib.h>


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


typedef NoRehashMap <
    uint64_t,
    DataCounted,
    boost::hash<uint64_t>, std::equal_to<uint64_t>,
    BlockStorageAllocator<std::pair<const uint64_t, uint64_t>>,
    MallocAllocator<void* >> MapBlockStorageType;

typedef NoRehashMap<uint64_t, DataCounted> MapDefaultAllocatorsType;


#define checkTrue(expression) checkTrueFunction(expression, "", #expression, __FILE__, __LINE__)
#define checkTrueMsg(expression, message) checkTrueFunction(expression, message, #expression, __FILE__, __LINE__)


void checkTrueFunction(bool value, const char *message,
                       const char *expression, const char *file, size_t line)
{
    if(!value)
    {
        const char *msg = message;

        if(msg == nullptr || msg[0] == 0)
        {
            msg = "check failed";
        }

        std::cout << msg << ": " << expression << std::endl;
        std::cout << file << ": " << line << std::endl;
        exit(-1);
    }
}


template<typename NodeAllocator>
void checkNodeAllocator(const NodeAllocator &nodeAllocator, size_t expectedAllocatedCount)
{
}


template<>
void checkNodeAllocator<MapBlockStorageType::NodeAllocatorType>(
    const MapBlockStorageType::NodeAllocatorType &nodeAllocator,
    size_t expectedAllocatedCount)
{
    const MapBlockStorageType::NodeAllocatorType::BlockStorageType &storage = nodeAllocator.getStorage();

    size_t allocatedItems, freeItems;
    storage.getStorageInfo(allocatedItems, freeItems);

    if(allocatedItems != expectedAllocatedCount)
    {
        std::cout << "invalid allocated count: " << allocatedItems <<
                  "   expected: " << expectedAllocatedCount << std::endl;
        exit(-1);
    }
}


template<typename MapType>
void testNoRehashMap()
{
    MapType m;

    std::vector<uint64_t> keys;

    const int itemCount = 300000;

    checkTrue(randomVector(keys, itemCount));

    for(int i = 0; i < itemCount; ++i)
    {
        DataCounted d;
        d.key = keys[i];

        std::pair<typename MapType::iterator, bool> p = m.insert(typename MapType::value_type(keys[i], d));

        checkTrue(p.second);

        checkTrue(m.size() == static_cast<size_t>(i + 1));
    }

    checkNodeAllocator(m.getNodeAllocator(), itemCount);

    for(int i = 0; i < itemCount; ++i)
    {
        checkTrue(m[keys[i]].key == keys[i]);
    }

    checkNodeAllocator(m.getNodeAllocator(), itemCount);

    for(int k = 0; k < 4; ++k)
    {
        for(int i = k % 2; i < itemCount; i += 2)
        {
            checkTrue(m.erase(keys[i]) == 1);
        }

        checkTrue(m.size() == itemCount / 2);

        checkNodeAllocator(m.getNodeAllocator(), itemCount / 2);

        for(int i = k % 2; i < itemCount; i += 2)
        {
            DataCounted d;
            d.key = keys[i];

            std::pair<typename MapType::iterator, bool> p = m.insert(typename MapType::value_type(keys[i], d));

            checkTrue(p.second);
        }

        checkTrue(m.size() == itemCount);

        checkNodeAllocator(m.getNodeAllocator(), itemCount);
    }


    for(int i = 1; i < itemCount; i += 2)
    {
        checkTrue(m.erase(keys[i]) == 1);
    }

    checkTrue(m.size() == itemCount / 2);

    checkNodeAllocator(m.getNodeAllocator(), itemCount / 2);


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
            checkTrue(iter->first == iter->second.key);

            checkTrue(keySet.count(iter->second.key) == 1);

            checkTrue(keySet.erase(iter->second.key) == 1);
        }

        checkTrue(keySet.size() == 0);
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
            checkTrue(v.first == v.second.key);

            checkTrue(keySet.count(v.second.key) == 1);

            checkTrue(keySet.erase(v.second.key) == 1);
        }

        checkTrue(keySet.size() == 0);
    }

    checkNodeAllocator(m.getNodeAllocator(), itemCount / 2);
}

int main()
{
#ifdef NDEBUG
    assert(0); // check that asserts don't work.
    std::cout << "asserts disabled" << std::endl;
#else
    std::cout << "asserts enabled" << std::endl;
#endif


    testNoRehashMap<MapBlockStorageType>();

    testNoRehashMap<MapDefaultAllocatorsType>();


    if(DataCounted::ctrCallCount != DataCounted::desCallCount)
    {
        std::cout << "constructor calls: " << DataCounted::ctrCallCount << std::endl;
        std::cout << "destructor calls: " << DataCounted::desCallCount << std::endl;
        return -1;
    }

    std::cout << "all tests passed" << std::endl;

    return 0;
}
