#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <cstdint>
#include <chrono>
#include <random>
#include <iostream>
#include <limits>


inline uint64_t getTicks()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
}


inline uint64_t randomUInt64()
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;
    return dis(gen);
}


template<typename T>
T randomValue()
{
    return static_cast<T>(randomUInt64());
}


struct BenchmarkParameters
{
    int64_t itemCount = 0;
    int64_t iterCount = 0;
    uint64_t ticks = 0;
    void *arg0 = nullptr;
    void *arg1 = nullptr;
    const char * testName = nullptr;
};


struct BenchmarkSet
{
    std::vector<BenchmarkParameters> params;
    std::vector<std::string> prefixes;
};


struct BenchmarkSingle
{
    BenchmarkParameters params;
    std::vector<std::string> prefixes;
};


bool randomVector(std::vector<uint64_t> &keys, int64_t itemCount);

bool randomVector(std::vector<std::string> &keys, int64_t itemCount, int64_t keyLength);


bool resultToFile(const BenchmarkSingle &bench);


struct Data
{
    uint64_t key;
    uint64_t a, b, c, d;
};


bool resultToFile(const BenchmarkSet &benchSet);


// copy of benchSet is created in argument
template <bool (*Fun)(BenchmarkParameters &params)>
bool runBenchmarkSet(BenchmarkSet benchSet)
{
    for(BenchmarkParameters &params : benchSet.params)
    {
        if(!Fun(params))
        {
            return false;
        }
    }

    resultToFile(benchSet);

    return true;
}


void randomString(const char *s, size_t length);

std::string randomString(size_t length);


#endif

