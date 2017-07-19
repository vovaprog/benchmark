#include <QueueBenchmark.h>
#include <Tools.h>

#include <queue>
#include <list>
#include <mutex>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/queue.hpp>
#include <tbb/concurrent_queue.h>

template<typename QueueType>
bool queueBench(BenchmarkParameters &params)
{
    QueueType q;

    for (int i=0;i<params.itemCount;++i)
    {
        q.push(i);
    }

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    for (int i=0;i<params.iterCount;++i)
    {
        uint64_t x = q.front();
        q.pop();
        q.push(x);
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


template<typename QueueType>
bool queueBenchPopParameter(BenchmarkParameters &params)
{
    QueueType q;

    for (int i=0;i<params.itemCount;++i)
    {
        q.push(i);
    }

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    for (int i=0;i<params.iterCount;++i)
    {
        uint64_t x;
        q.pop(x);
        q.push(x);
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


template<typename QueueType>
bool queueBenchTryPop(BenchmarkParameters &params)
{
    QueueType q;

    for (int i=0;i<params.itemCount;++i)
    {
        q.push(i);
    }

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    for (int i=0;i<params.iterCount;++i)
    {
        uint64_t x;
        q.try_pop(x);
        q.push(x);
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


template<typename QueueType>
bool queueBenchTryPushTryPop(BenchmarkParameters &params)
{
    QueueType q;

    for (int i=0;i<params.itemCount;++i)
    {
        q.push(i);
    }

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    for (int i=0;i<params.iterCount;++i)
    {
        uint64_t x;
        q.try_pop(x);
        q.try_push(x);
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


template<typename QueueType>
bool queueBenchLock(BenchmarkParameters &params)
{
    QueueType q;
    std::mutex mtx;

    for (int i=0;i<params.itemCount;++i)
    {
        q.push(i);
    }

    uint64_t ticks = getTicks();

    //=================================================================================
    // benchmark code
    //=================================================================================

    for (int i=0;i<params.iterCount;++i)
    {
        uint64_t x;
        {
            std::lock_guard<std::mutex> lock(mtx);
            x = q.front();
            q.pop();
        }
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(x);
        }
    }

    //=================================================================================
    // benchmark code
    //=================================================================================

    params.ticks = getTicks() - ticks;

    return true;
}


bool stdQueueBenchmark(BenchmarkParameters &params)
{
    return queueBench<std::queue<uint64_t>>(params);
}


bool stdQueueListBenchmark(BenchmarkParameters &params)
{
    return queueBench<std::queue<uint64_t, std::list<uint64_t>>>(params);
}


bool boostSpscQueueBenchmark(BenchmarkParameters &params)
{
    return queueBench<boost::lockfree::spsc_queue<uint64_t, boost::lockfree::capacity<110000>>>(params);
}


bool boostLockfreeQueueBenchmark(BenchmarkParameters &params)
{
    return queueBenchPopParameter<boost::lockfree::queue<uint64_t>>(params);
}


bool stdQueueLockBenchmark(BenchmarkParameters &params)
{
    return queueBenchLock<std::queue<uint64_t>>(params);
}


bool tbbConcurrentQueueBenchmark(BenchmarkParameters &params)
{
    return queueBenchTryPop<tbb::concurrent_queue<uint64_t>>(params);
}


bool tbbConcurrentBoundedQueueBenchmark(BenchmarkParameters &params)
{
    return queueBenchTryPushTryPop<tbb::concurrent_bounded_queue<uint64_t>>(params);
}


bool queueBenchmark()
{
    std::cout << "===== queue benchmark =====" << std::endl;

    BenchmarkSingle bench;
    bench.prefixes.push_back("queue");    

    bench.params.itemCount = 100000;
    bench.params.iterCount = 10000000;

    bench.params.testName = "std";
    std::cout << bench.params.testName << std::endl;
    stdQueueBenchmark(bench.params);
    resultToFile(bench);

    bench.params.testName = "std list";
    std::cout << bench.params.testName << std::endl;
    stdQueueListBenchmark(bench.params);
    resultToFile(bench);

    bench.params.testName = "spsc_queue";
    std::cout << bench.params.testName << std::endl;
    boostSpscQueueBenchmark(bench.params);
    resultToFile(bench);

    bench.params.testName = "lockfree";
    std::cout << bench.params.testName << std::endl;
    boostLockfreeQueueBenchmark(bench.params);
    resultToFile(bench);

    bench.params.testName = "std lock";
    std::cout << bench.params.testName << std::endl;
    stdQueueLockBenchmark(bench.params);
    resultToFile(bench);

    bench.params.testName = "tbb concurrent_queue";
    std::cout << bench.params.testName << std::endl;
    tbbConcurrentQueueBenchmark(bench.params);
    resultToFile(bench);

    bench.params.testName = "tbb concurrent_bounded_queue";
    std::cout << bench.params.testName << std::endl;
    tbbConcurrentBoundedQueueBenchmark(bench.params);
    resultToFile(bench);

    return true;
}

