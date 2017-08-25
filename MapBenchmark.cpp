#include <MapBenchmark.h>

#include <Tools.h>
#include <IntrusiveMapBenchmark.h>

#include <map>
#include <unordered_map>
#include <deque>
#include <boost/container/map.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <tbb/concurrent_hash_map.h>

#include <NoRehashMap/NoRehashMap.h>
#include <NoRehashMap/BlockStorageAllocator.h>
#include <NoRehashMap/MallocAllocator.h>

template<typename T>
struct HashNone
{
    size_t operator()(T x) const
    {
        return x;
    }
};

template<typename MapType>
bool benchMapFind_fillMap(BenchmarkParameters &params, MapType &m)
{
    m.clear();

    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);

    for(int i = 0; i < params.itemCount; ++i)
    {
        Data d;
        d.key = keys[i];
        m[d.key] = d;
        //m.insert(typename MapType::value_type(d.key, d));
    }

    return true;
}


// fillMap specialization for boost::flat_map,
// needed because insertion into boost::flat_map is very slow.
template<>
bool benchMapFind_fillMap<boost::container::flat_map<uint64_t, Data>>(
            BenchmarkParameters &params, boost::container::flat_map<uint64_t, Data> &m)
{
    m.clear();

    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);
    std::vector<boost::container::flat_map<uint64_t, Data>::value_type> values;

    values.reserve(params.itemCount);

    for(int i = 0; i < params.itemCount; ++i)
    {
        Data d;
        d.key = keys[i];

        boost::container::flat_map<uint64_t, Data>::value_type p;
        p.first = keys[i];
        p.second = d;

        values.push_back(p);
    }

    m.reserve(params.itemCount);

    // values should be sorted before insertion
    std::sort(values.begin(), values.end(),
              [](boost::container::flat_map<uint64_t, Data>::value_type & v0,
                 boost::container::flat_map<uint64_t, Data>::value_type & v1)
    {
        return v0.first < v1.first;
    });

    // ordered_unique_range_t indicates that values are sorted and unique
    m.insert(boost::container::ordered_unique_range_t(), values.begin(), values.end());

    return true;
}


template<typename MapType>
bool benchMapFind(BenchmarkParameters &params)
{
    MapType m;

    if(!benchMapFind_fillMap(params, m))
    {
        return false;
    }

    if(m.size() != static_cast<uint64_t>(params.itemCount))
    {
        return false;
    }

    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);

    int64_t searchCounter = 0;

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    while(searchCounter < params.iterCount)
    {
        for(int i = 0; i < params.itemCount && searchCounter < params.iterCount; ++i, ++searchCounter)
        {
            auto iter = m.find(keys[i]);

            if(iter == m.end() || iter->second.key != keys[i])
            {
                return false;
            }
        }
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


bool benchMapFindTbbConcurrentMap(BenchmarkParameters &params)
{
    typedef tbb::concurrent_hash_map<uint64_t, Data> MapType;
    MapType m;

    m.clear();

    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);

    for(int i = 0; i < params.itemCount; ++i)
    {
        Data d;
        d.key = keys[i];
        if(!m.insert(MapType::value_type(keys[i], d)))
        {
            return false;
        }
    }


    if(m.size() != static_cast<uint64_t>(params.itemCount))
    {
        return false;
    }

    int64_t searchCounter = 0;

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    while(searchCounter < params.iterCount)
    {
        for(int i = 0; i < params.itemCount && searchCounter < params.iterCount; ++i, ++searchCounter)
        {
            MapType::const_accessor acc;

            if(!m.find(acc, keys[i]) || acc->second.key != keys[i])
            {
                return false;
            }
        }
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


bool benchStdMapFind(BenchmarkParameters &params)
{
    bool result = benchMapFind<std::map<uint64_t, Data>>(params);
    params.testName = "std map";
    return result;
}


bool benchBoostMapFind(BenchmarkParameters &params)
{
    bool result = benchMapFind<boost::container::map<uint64_t, Data>>(params);
    params.testName = "boost map";
    return result;
}


bool benchBoostFlatMapFind(BenchmarkParameters &params)
{
    bool result = benchMapFind<boost::container::flat_map<uint64_t, Data>>(params);
    params.testName = "boost flat_map";
    return result;
}


bool benchStdUnorderedMapFind(BenchmarkParameters &params)
{
    bool result = benchMapFind<std::unordered_map<uint64_t, Data>>(params);
    params.testName = "std unordered_map";
    return result;
}


bool benchStdUnorderedMapFindNoHash(BenchmarkParameters &params)
{
    bool result = benchMapFind<std::unordered_map<uint64_t, Data, HashNone<uint64_t>>>(params);
    params.testName = "std unordered_map no hash";
    return result;
}


bool benchBoostUnorderedMapFind(BenchmarkParameters &params)
{
    bool result = benchMapFind<boost::unordered_map<uint64_t, Data>>(params);
    params.testName = "boost unordered_map";
    return result;
}


bool benchBoostUnorderedMapFindNoHash(BenchmarkParameters &params)
{
    bool result = benchMapFind<boost::unordered_map<uint64_t, Data, HashNone<uint64_t>>>(params);
    params.testName = "boost unordered_map no hash";
    return result;
}


bool benchTbbConcurrentHashMap(BenchmarkParameters &params)
{
    bool result = benchMapFindTbbConcurrentMap(params);
    params.testName = "tbb concurrent_hash_map";
    return result;
}


bool benchNoRehashMap(BenchmarkParameters &params)
{
    typedef NoRehashMap<
            uint64_t,
            Data,
            boost::hash<int>, std::equal_to<int>,
            BlockStorageAllocator<std::pair<const uint64_t, Data>>,
            MallocAllocator<void*>> NoRehashMapType;

    bool result = benchMapFind<NoRehashMapType>(params);
    params.testName = "no rehash";
    return result;
}


template<typename T>
bool benchSortedArray(BenchmarkParameters &params)
{
    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);

    T v;

    for(int i = 0; i < params.itemCount; ++i)
    {
        Data d;
        d.key = keys[i];
        v.push_back(d);
    }

    std::sort(v.begin(), v.end(),
              [](const Data & d0, const Data & d1)
    {
        return d0.key < d1.key;
    });

    int64_t searchCounter = 0;

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    while(searchCounter < params.iterCount)
    {
        for(int i = 0; i < params.itemCount && searchCounter < params.iterCount; ++i, ++searchCounter)
        {
            auto iter = std::lower_bound(v.begin(), v.end(), keys[i],
                                         [](const Data & d, uint64_t key)
            {
                return d.key < key;
            });

            if(iter == v.end() || iter->key != keys[i])
            {
                return false;
            }
        }
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


bool benchSortedVector(BenchmarkParameters &params)
{
    bool result = benchSortedArray<std::vector<Data>>(params);
    params.testName = "std vector";
    return result;
}


bool benchSortedDeque(BenchmarkParameters &params)
{
    bool result = benchSortedArray<std::deque<Data>>(params);
    params.testName = "std deque";
    return result;
}


template<typename MapType>
bool benchMapInsert(BenchmarkParameters &params)
{
    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    MapType m;

    for(int i = 0; i < params.itemCount; ++i)
    {
        Data d;
        d.key = keys[i];
        m[d.key] = d;
        //m.insert(typename MapType::value_type(d.key, d));
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    if(m.size() != static_cast<uint64_t>(params.itemCount))
    {
        return false;
    }

    return true;
}


bool benchStdMapInsert(BenchmarkParameters &params)
{
    bool result = benchMapInsert<std::map<uint64_t, Data>>(params);
    params.testName = "std map insert";
    return result;
}


bool benchBoostMapInsert(BenchmarkParameters &params)
{
    bool result = benchMapInsert<boost::container::map<uint64_t, Data>>(params);
    params.testName = "boost map insert";
    return result;
}


bool benchStdUnorderedMapInsert(BenchmarkParameters &params)
{
    bool result = benchMapInsert<std::unordered_map<uint64_t, Data>>(params);
    params.testName = "std unordered_map insert";
    return result;
}


bool benchBoostUnorderedMapInsert(BenchmarkParameters &params)
{
    bool result = benchMapInsert<boost::unordered_map<uint64_t, Data>>(params);
    params.testName = "boost unordered_map insert";
    return result;
}


bool benchNoRehashMapInsert(BenchmarkParameters &params)
{
    bool result = benchMapInsert<NoRehashMap<uint64_t, Data>>(params);
    params.testName = "no rehash insert";
    return result;
}


bool benchBoostFlatMapFill(BenchmarkParameters &params)
{
    boost::container::flat_map<uint64_t, Data> m;

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    bool result = benchMapFind_fillMap(params, m);

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    if(m.size() != static_cast<uint64_t>(params.itemCount))
    {
        return false;
    }

    params.testName = "boost flat_map fill";
    return result;
}


bool createBenchmarkParameters(int64_t itemCountStart, int64_t itemCountEnd, int64_t itemCountStep,
                               int64_t iterCount,
                               BenchmarkSet &benchSet,
                               std::vector<uint64_t> &keys)
{
    if(!randomVector(keys, itemCountEnd))
    {
        std::cout << "randomVector failed\n";
        return false;
    }

    benchSet.params.clear();

    for(int i = itemCountStart; i <= itemCountEnd; i += itemCountStep)
    {
        BenchmarkParameters params;
        params.itemCount = i;
        params.iterCount = iterCount;
        params.arg0 = &keys;

        benchSet.params.push_back(params);
    }

    return true;
}


bool benchMapsFind(int64_t itemCountStart, int64_t itemCountEnd, int64_t itemCountStep)
{
    const int64_t iterCount = 100000;

    BenchmarkSet benchSet;
    std::vector<uint64_t> keys;

    if(!createBenchmarkParameters(itemCountStart, itemCountEnd, itemCountStep,
                                  iterCount, benchSet, keys))
    {
        return false;
    }

    benchSet.prefixes.push_back("map find");
    benchSet.prefixes.push_back(std::to_string(itemCountEnd));

    std::cout << "std map" << std::endl;
    if(!runBenchmarkSet<benchStdMapFind>(benchSet))
    {
        return false;
    }

    std::cout << "boost map" << std::endl;
    if(!runBenchmarkSet<benchBoostMapFind>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapFind>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map no hash" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapFindNoHash>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapFind>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map no hash" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapFindNoHash>(benchSet))
    {
        return false;
    }

    std::cout << "sorted vector" << std::endl;
    if(!runBenchmarkSet<benchSortedVector>(benchSet))
    {
        return false;
    }

    std::cout << "sorted deque" << std::endl;
    if(!runBenchmarkSet<benchSortedDeque>(benchSet))
    {
        return false;
    }

    std::cout << "boost flat_map" << std::endl;
    if(!runBenchmarkSet<benchBoostFlatMapFind>(benchSet))
    {
        return false;
    }

    std::cout << "boost intrusive set" << std::endl;
    if(!runBenchmarkSet<intrusiveSetFindBenchmark>(benchSet))
    {
        return false;
    }

    std::cout << "boost intrusive avl_set" << std::endl;
    if(!runBenchmarkSet<intrusiveAvlSetFindBenchmark>(benchSet))
    {
        return false;
    }

    std::cout << "tbb concurrent_hash_map" << std::endl;
    if(!runBenchmarkSet<benchTbbConcurrentHashMap>(benchSet))
    {
        return false;
    }

    std::cout << "no rehash" << std::endl;
    if(!runBenchmarkSet<benchNoRehashMap>(benchSet))
    {
        return false;
    }

    return true;
}


bool benchMapsInsert(int64_t itemCountStart, int64_t itemCountEnd, int64_t itemCountStep)
{
    const int64_t iterCount = 100000;

    BenchmarkSet benchSet;
    std::vector<uint64_t> keys;

    if(!createBenchmarkParameters(itemCountStart, itemCountEnd, itemCountStep,
                                  iterCount, benchSet, keys))
    {
        return false;
    }

    benchSet.prefixes.push_back("map insert");
    benchSet.prefixes.push_back(std::to_string(itemCountEnd));

    std::cout << "std map" << std::endl;
    if(!runBenchmarkSet<benchStdMapInsert>(benchSet))
    {
        return false;
    }

    std::cout << "boost map" << std::endl;
    if(!runBenchmarkSet<benchBoostMapInsert>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapInsert>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapInsert>(benchSet))
    {
        return false;
    }

    std::cout << "no rehash" << std::endl;
    if(!runBenchmarkSet<benchNoRehashMapInsert>(benchSet))
    {
        return false;
    }

    std::cout << "boost flat_map" << std::endl;
    if(!runBenchmarkSet<benchBoostFlatMapFill>(benchSet))
    {
        return false;
    }

    return true;
}


bool mapFindBenchmark()
{
    std::cout << "===== map find benchmark =====" << std::endl;

    if(!benchMapsFind(5, 350, 1))
    {
        return false;
    }

    if(!benchMapsFind(50, 3500, 30))
    {
        return false;
    }

    if(!benchMapsFind(1000, 100000, 2000))
    {
        return false;
    }

    return true;
}


bool mapInsertBenchmark()
{
    std::cout << "===== map insert benchmark =====" << std::endl;

    if(!benchMapsInsert(5, 100000, 500))
    {
        return false;
    }

    return true;
}


template<class MapType>
bool benchInsertItem(BenchmarkSet &benchSet, int itemCount, const char *testName)
{
    MapType m;

    benchSet.params.clear();
    benchSet.params.reserve(itemCount);

    for(int i=1;i<=itemCount;++i)
    {
        Data d;
        d.key = i;

        uint64_t ticks = getTicksNano();

        m.insert(typename MapType::value_type(d.key, d));

        ticks = getTicksNano() - ticks;

        BenchmarkParameters params;
        params.itemCount = i;
        params.ticks = ticks;
        params.testName = testName;

        benchSet.params.push_back(params);
    }

    resultToFile(benchSet);

    return true;
}


bool mapInsertItemBenchmark()
{
    std::cout << "===== map insert item benchmark =====" << std::endl;

    const int itemCount = 100000;

    BenchmarkSet benchSet;
    benchSet.prefixes.push_back("map insert item");

    std::cout << "boost unordered_map" << std::endl;
    benchInsertItem<boost::unordered_map<uint64_t, Data>>(benchSet, itemCount, "boost unordered_map");

    typedef NoRehashMap<
            uint64_t,
            Data,
            boost::hash<int>, std::equal_to<int>,
            BlockStorageAllocator<std::pair<const uint64_t, Data>>,
            MallocAllocator<void*>> NoRehashMapType;

    std::cout << "no rehash map" << std::endl;
    benchInsertItem<NoRehashMapType>(benchSet, itemCount, "no rehash map");

    std::cout << "std map" << std::endl;
    benchInsertItem<std::map<uint64_t, Data>>(benchSet, itemCount, "std map");

    return true;
}

