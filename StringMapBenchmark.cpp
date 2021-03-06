#include <StringMapBenchmark.h>

#include <iostream>
#include <map>
#include <unordered_map>
#include <string.h>
#include <boost/container/map.hpp>
#include <boost/unordered_map.hpp>

#include <StringUtils.h>

template<typename MapType, typename KeyType>
bool benchStringMapFind(BenchmarkParameters &params, std::vector<KeyType> &keys)
{
    MapType m;

    for(int i = 0; i < params.itemCount; ++i)
    {
        m[keys[i]] = Data();
    }

    if((int64_t)m.size() != params.itemCount)
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
            auto iter = m.find(keys[i]);

            if(iter == m.end())
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


bool benchStdMapCharPointer(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind <
                  std::map<const char*, Data, CharPointerLess>, const char* > (params, keys);

    params.testName = "std map char p";
    return result;
}


bool benchStdMapString(BenchmarkParameters &params)
{
    std::vector<std::string> &keys = *static_cast<std::vector<std::string> *>(params.arg1);

    bool result = benchStringMapFind <
                  std::map<std::string, Data>, std::string > (params, keys);

    params.testName = "std map string";
    return result;
}


bool benchStdUnorderedMapCharPointer(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind <
                  std::unordered_map<const char*, Data, CharPointerHashCombine, CharPointerEqual >,
                  const char* > (params, keys);

    params.testName = "std unordered_map char p combine";
    return result;
}


bool benchStdUnorderedMapCharPointerFnv1(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind <
                  std::unordered_map<const char*, Data, CharPointerHashFnv1, CharPointerEqual >,
                  const char* > (params, keys);

    params.testName = "std unordered_map char p fnv1";
    return result;
}


bool benchStdUnorderedMapCharPointerMurmur(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind <
                  std::unordered_map<const char*, Data, CharPointerHashMurmur, CharPointerEqual >,
                  const char* > (params, keys);

    params.testName = "std unordered_map char p murmur";
    return result;
}


bool benchStdUnorderedMapString(BenchmarkParameters &params)
{
    std::vector<std::string> &keys = *static_cast<std::vector<std::string> *>(params.arg1);

    bool result = benchStringMapFind <
                  std::unordered_map<std::string, Data>, std::string > (params, keys);

    params.testName = "std unordered_map string";
    return result;
}


bool benchBoostMapCharPointer(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind <
                  boost::container::map<const char*, Data, CharPointerLess >,
                  const char * > (params, keys);

    params.testName = "boost map char p";
    return result;
}


bool benchBoostMapString(BenchmarkParameters &params)
{
    std::vector<std::string> &keys = *static_cast<std::vector<std::string> *>(params.arg1);

    bool result = benchStringMapFind <
                  boost::container::map<std::string, Data>, std::string > (params, keys);

    params.testName = "boost map string";
    return result;
}


bool benchBoostUnorderedMapCharPointer(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind<boost::unordered_map<
                  const char*, Data, CharPointerHashCombine, CharPointerEqual>,
                  const char*>(params, keys);

    params.testName = "boost unordered_map char p combine";
    return result;
}


bool benchBoostUnorderedMapCharPointerFnv1(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind<boost::unordered_map<
                  const char*, Data, CharPointerHashFnv1, CharPointerEqual>,
                  const char*>(params, keys);

    params.testName = "boost unordered_map char p fnv1";
    return result;
}


bool benchBoostUnorderedMapCharPointerMurmur(BenchmarkParameters &params)
{
    std::vector<const char*> &keys = *static_cast<std::vector<const char*> *>(params.arg0);

    bool result = benchStringMapFind<boost::unordered_map<
                  const char*, Data, CharPointerHashMurmur, CharPointerEqual>,
                  const char*>(params, keys);

    params.testName = "boost unordered_map char p murmur";
    return result;
}


bool benchBoostUnorderedMapString(BenchmarkParameters &params)
{
    std::vector<std::string> &keys = *static_cast<std::vector<std::string> *>(params.arg1);

    bool result = benchStringMapFind <
                  boost::unordered_map<std::string, Data>, std::string > (params, keys);

    params.testName = "boost unordered_map string";
    return result;
}


bool createBenchmarkParameters(int64_t itemCountStart, int64_t itemCountEnd, int64_t itemCountStep,
                               int64_t keyLength,
                               int64_t iterCount,
                               BenchmarkSet &benchSet,
                               std::vector<std::string> &keyStrings,
                               std::vector<const char*> &keyPointers)
{
    keyPointers.clear();

    if(!randomVector(keyStrings, itemCountEnd, keyLength))
    {
        return false;
    }

    for(std::string &s : keyStrings)
    {
        keyPointers.push_back(s.c_str());
    }

    if((int64_t)keyPointers.size() != itemCountEnd)
    {
        return false;
    }

    benchSet.params.clear();

    for(int i = itemCountStart; i <= itemCountEnd; i += itemCountStep)
    {
        BenchmarkParameters params;
        params.itemCount = i;
        params.iterCount = iterCount;
        params.arg0 = &keyPointers;
        params.arg1 = &keyStrings;

        benchSet.params.push_back(params);
    }

    return true;
}


bool stringMapFindBenchmark(int64_t itemCountStart, int64_t itemCountEnd, int64_t itemCountStep,
                            int64_t keyLength)
{
    const int64_t iterCount = 100000;

    BenchmarkSet benchSet;
    std::vector<std::string> keyStrings;
    std::vector<const char*> keyPointers;

    if(!createBenchmarkParameters(itemCountStart, itemCountEnd, itemCountStep, keyLength,
                                  iterCount, benchSet, keyStrings, keyPointers))
    {
        return false;
    }

    benchSet.prefixes.push_back("string map find");
    benchSet.prefixes.push_back(std::to_string(keyLength));
    benchSet.prefixes.push_back(std::to_string(itemCountEnd));


    std::cout << "std map char pointer" << std::endl;
    if(!runBenchmarkSet<benchStdMapCharPointer>(benchSet))
    {
        return false;
    }

    std::cout << "std map string" << std::endl;
    if(!runBenchmarkSet<benchStdMapString>(benchSet))
    {
        return false;
    }

    std::cout << "boost map char pointer" << std::endl;
    if(!runBenchmarkSet<benchBoostMapCharPointer>(benchSet))
    {
        return false;
    }

    std::cout << "boost map string" << std::endl;
    if(!runBenchmarkSet<benchBoostMapString>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map char pointer" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapCharPointer>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map char pointer fnv1" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapCharPointerFnv1>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map char pointer murmur" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapCharPointerMurmur>(benchSet))
    {
        return false;
    }

    std::cout << "std unordered_map string" << std::endl;
    if(!runBenchmarkSet<benchStdUnorderedMapString>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map char pointer" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapCharPointer>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map char pointer fnv1" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapCharPointerFnv1>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map char pointer murmur" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapCharPointerMurmur>(benchSet))
    {
        return false;
    }

    std::cout << "boost unordered_map string" << std::endl;
    if(!runBenchmarkSet<benchBoostUnorderedMapString>(benchSet))
    {
        return false;
    }

    return true;
}


bool stringMapFindBenchmark()
{
    std::cout << "===== string map find benchmark =====" << std::endl;

    if(!stringMapFindBenchmark(10, 300, 2, 15))
    {
        return false;
    }

    if(!stringMapFindBenchmark(10, 1000, 20, 15))
    {
        return false;
    }

    if(!stringMapFindBenchmark(10, 100000, 5000, 15))
    {
        return false;
    }

    return true;
}
