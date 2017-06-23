#include <SwitchBenchmark.h>
#include <Tools.h>
#include <SwitchBenchmarkGen.cpp>

#include <map>
#include <unordered_map>

namespace
{

bool runSwitch(BenchmarkParameters &params, int testIndex)
{
    SwitchFunction switchFun = switchFuns[testIndex];

    int count = 0;

    uint64_t ticks = getTicks();

    while(count < params.iterCount)
    {
        for(int i = 0; i < params.itemCount && count < params.iterCount; ++i, ++count)
        {
            if(switchFun(keys[i]) != vals[i])
            {
                return false;
            }
        }
    }

    params.ticks = getTicks() - ticks;

    return true;
}


template<typename MapType>
bool runMap(BenchmarkParameters &params)
{
    MapType m;

    for(int i = 0; i < params.itemCount; ++i)
    {
        m[keys[i]] = vals[i];
    }

    int count = 0;

    uint64_t ticks = getTicks();

    while(count < params.iterCount)
    {
        for(int i = 0; i < params.itemCount && count < params.iterCount; ++i, ++count)
        {
            auto iter = m.find(keys[i]);

            if(iter == m.end() || iter->second != vals[i])
            {
                return false;
            }
        }
    }

    params.ticks = getTicks() - ticks;

    return true;
}

const int iterCount = 1000000;

template<typename MapType>
bool runMap(const char *testName)
{
    BenchmarkSet bs;

    bs.prefixes.push_back("switch");

    for(size_t i = 0; i < funCount; ++i)
    {
        BenchmarkParameters params;
        params.itemCount = counts[i];
        params.iterCount = iterCount;
        params.testName = testName;

        if(!runMap<MapType>(params))
        {
            return false;
        }

        bs.params.push_back(params);
    }

    resultToFile(bs);

    return true;
}

} // namespace

bool switchBenchmark()
{
    std::cout << "===== switch benchmark =====" << std::endl;

    {
        std::cout << "switch" << std::endl;

        BenchmarkSet bs;

        bs.prefixes.push_back("switch");

        for(size_t i = 0; i < funCount; ++i)
        {
            BenchmarkParameters params;
            params.itemCount = counts[i];
            params.iterCount = iterCount;
            params.testName = "switch";

            if(!runSwitch(params, i))
            {
                return false;
            }

            bs.params.push_back(params);
        }

        resultToFile(bs);
    }


    std::cout << "map" << std::endl;
    if(!runMap<std::map<uint32_t, uint32_t>>("map"))
    {
        return false;
    }


    std::cout << "unordered_map" << std::endl;
    if(!runMap<std::unordered_map<uint32_t, uint32_t>>("unordered_map"))
    {
        return false;
    }

    return true;
}


