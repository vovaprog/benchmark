#include <Tools.h>

#include <iostream>


template<typename T, long long int iterCount>
struct OperationAdd
{
    static T __attribute__((optimize("O0"))) calculate(T x, T inc)
    {
        T result;
        for(int i = 0; i < iterCount; ++i)
        {
            result = x + inc;
        }
        return result;
    }
};


template<typename T, long long int iterCount>
struct OperationOr
{
    static T __attribute__((optimize("O0"))) calculate(T x, T inc)
    {
        T result;
        for(int i = 0; i < iterCount; ++i)
        {
            result = x | inc;
        }
        return result;
    }
};


template<typename T, long long int iterCount>
struct OperationMul
{
    static T __attribute__((optimize("O0"))) calculate(T x, T inc)
    {
        T result;
        for(int i = 0; i < iterCount; ++i)
        {
            result = x * inc;
        }
        return result;
    }
};


template<typename T, long long int iterCount>
struct OperationDiv
{
    static T __attribute__((optimize("O0"))) calculate(T x, T inc)
    {
        T result;
        for(int i = 0; i < iterCount; ++i)
        {
            result = x / inc;
        }
        return result;
    }
};


template<typename Operation, typename T>
void arithmeticBenchmark(BenchmarkParameters &params)
{
    T x = randomValue<T>();
    T inc = static_cast<uint64_t>(randomValue<T>()) % 1000 + 2;

    uint64_t ticks = getTicks();

    x = Operation::calculate(x, inc);
    (void)x;

    params.ticks = getTicks() - ticks;
}


bool arithmeticBenchmark()
{
    std::cout << "===== arithmetic benchmark =====" << std::endl;

    const long long int iterCount = 100000000;

    BenchmarkSingle bench;
    bench.prefixes.push_back("arithmetic");


    std::cout << "add" << std::endl;

    bench.params.testName = "add uint08";
    arithmeticBenchmark<OperationAdd<uint8_t, iterCount>, uint8_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "add uint16";
    arithmeticBenchmark<OperationAdd<uint16_t, iterCount>, uint16_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "add uint32";
    arithmeticBenchmark<OperationAdd<uint32_t, iterCount>, uint32_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "add uint64";
    arithmeticBenchmark<OperationAdd<uint64_t, iterCount>, uint64_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "add float";
    arithmeticBenchmark<OperationAdd<float, iterCount>, float>(bench.params);
    resultToFile(bench);

    bench.params.testName = "add double";
    arithmeticBenchmark<OperationAdd<double, iterCount>, double>(bench.params);
    resultToFile(bench);

    bench.params.testName = "add long double";
    arithmeticBenchmark<OperationAdd<long double, iterCount>, long double>(bench.params);
    resultToFile(bench);


    std::cout << "mul" << std::endl;

    bench.params.testName = "mul uint08";
    arithmeticBenchmark<OperationMul<uint8_t, iterCount>, uint8_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "mul uint16";
    arithmeticBenchmark<OperationMul<uint16_t, iterCount>, uint16_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "mul uint32";
    arithmeticBenchmark<OperationMul<uint32_t, iterCount>, uint32_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "mul uint64";
    arithmeticBenchmark<OperationMul<uint64_t, iterCount>, uint64_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "mul float";
    arithmeticBenchmark<OperationMul<float, iterCount>, float>(bench.params);
    resultToFile(bench);

    bench.params.testName = "mul double";
    arithmeticBenchmark<OperationMul<double, iterCount>, double>(bench.params);
    resultToFile(bench);

    bench.params.testName = "mul long double";
    arithmeticBenchmark<OperationMul<long double, iterCount>, long double>(bench.params);
    resultToFile(bench);


    std::cout << "div" << std::endl;

    bench.params.testName = "div uint08";
    arithmeticBenchmark<OperationDiv<uint8_t, iterCount>, uint8_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "div uint16";
    arithmeticBenchmark<OperationDiv<uint16_t, iterCount>, uint16_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "div uint32";
    arithmeticBenchmark<OperationDiv<uint32_t, iterCount>, uint32_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "div uint64";
    arithmeticBenchmark<OperationDiv<uint64_t, iterCount>, uint64_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "div float";
    arithmeticBenchmark<OperationDiv<float, iterCount>, float>(bench.params);
    resultToFile(bench);

    bench.params.testName = "div double";
    arithmeticBenchmark<OperationDiv<double, iterCount>, double>(bench.params);
    resultToFile(bench);

    bench.params.testName = "div long double";
    arithmeticBenchmark<OperationDiv<long double, iterCount>, long double>(bench.params);
    resultToFile(bench);


    std::cout << "or" << std::endl;

    bench.params.testName = "or uint08";
    arithmeticBenchmark<OperationOr<uint8_t, iterCount>, uint8_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "or uint16";
    arithmeticBenchmark<OperationOr<uint16_t, iterCount>, uint16_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "or uint32";
    arithmeticBenchmark<OperationOr<uint32_t, iterCount>, uint32_t>(bench.params);
    resultToFile(bench);

    bench.params.testName = "or uint64";
    arithmeticBenchmark<OperationOr<uint64_t, iterCount>, uint64_t>(bench.params);
    resultToFile(bench);

    return true;
}

