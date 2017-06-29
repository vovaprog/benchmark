#include <stdlib.h>
#include <strings.h>
#include <iostream>
#include <boost/version.hpp>

#include <MapBenchmark.h>
#include <IntrusiveMapBenchmark.h>
#include <ArithmeticBenchmark.h>
#include <StringMapBenchmark.h>
#include <SwitchBenchmark.h>


int main(int argc, char *argv[])
{
    srand(time(nullptr));

    if(argc > 1)
    {
        if(strcasecmp(argv[1], "GetBoostVersion") == 0)
        {
            std::cout << BOOST_LIB_VERSION << std::endl;
        }
        else
        {
            for(int i = 1; i < argc; ++i)
            {
                if(strcasecmp(argv[i], "MapFind") == 0)
                {
                    if(!mapFindBenchmark())
                    {
                        std::cout << "map find benchmark failed" << std::endl;
                        return -1;
                    }
                }
                else if(strcasecmp(argv[i], "MapInsert") == 0)
                {
                    if(!mapInsertBenchmark())
                    {
                        std::cout << "map insert benchmark failed" << std::endl;
                        return -1;
                    }
                }
                else if(strcasecmp(argv[i], "Arithmetic") == 0)
                {
                    if(!arithmeticBenchmark())
                    {
                        std::cout << "arithmetic benchmark failed" << std::endl;
                        return -1;
                    }
                }
                else if(strcasecmp(argv[i], "StringMap") == 0)
                {
                    if(!stringMapFindBenchmark())
                    {
                        std::cout << "string map find benchmark failed" << std::endl;
                        return -1;
                    }
                }
                else if(strcasecmp(argv[i], "Switch") == 0)
                {
                    if(!switchBenchmark())
                    {
                        std::cout << "switch benchmark failed" << std::endl;
                        return -1;
                    }
                }
                else
                {
                    std::cout << "invalid benchmark name: " << argv[i] << std::endl;
                    return -1;
                }
            }
        }
    }
    else
    {
        std::cout << "usage: benchmark [benchmark name]\n"
            "benchmark names: MapFind, MapInsert, Arithmetic, StringMap, Switch\n"
            "if benchmark name is not specified all benchmarks will be executed" << std::endl;

        if(!mapFindBenchmark())
        {
            std::cout << "map find benchmark failed" << std::endl;
            return -1;
        }

        if(!mapInsertBenchmark())
        {
            std::cout << "map insert benchmark failed" << std::endl;
            return -1;
        }

        if(!arithmeticBenchmark())
        {
            std::cout << "arithmetic benchmark failed" << std::endl;
            return -1;
        }

        if(!stringMapFindBenchmark())
        {
            std::cout << "string map find benchmark failed" << std::endl;
            return -1;
        }

        if(!switchBenchmark())
        {
            std::cout << "map insert benchmark failed" << std::endl;
            return -1;
        }
    }

    return 0;
}

