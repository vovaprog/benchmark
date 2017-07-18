#include <AlignBenchmark.h>
#include <Tools.h>


void alignBench(BenchmarkParameters &params, uint64_t *arr)
{
    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    for(int k = 0; k < params.iterCount; ++k)
    {
        for(int i = 0; i < params.itemCount; ++i)
        {
            arr[i] += arr[params.itemCount - 1 - i];
        }
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;
}


bool alignBenchmark()
{
    std::cout << "===== align benchmark =====" << std::endl;

    BenchmarkSingle bench;
    bench.prefixes.push_back("align");

    bench.params.itemCount = 50;
    bench.params.iterCount = 10000000;

    uintptr_t intPtr = (uintptr_t)malloc((bench.params.itemCount + 2) * 8);

    for(int i = 0; i < bench.params.itemCount + 2; ++i)
    {
        (reinterpret_cast<uint64_t*>(intPtr))[i] = i;
    }

    if(intPtr % 8 != 0)
    {
        intPtr += 8;
        intPtr -= intPtr % 8;
    }

    std::string testNamePrefix("align");

    for(int i = 0; i < 8; ++i)
    {
        std::string testName = testNamePrefix + std::to_string(i);

        std::cout << testName << std::endl;
        bench.params.testName = testName.c_str();

        alignBench(bench.params, reinterpret_cast<uint64_t*>(intPtr + i));

        resultToFile(bench);
    }

    return true;
}

