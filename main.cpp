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
                    mapFindBenchmark();
                }
                else if(strcasecmp(argv[i], "MapInsert") == 0)
                {
                    mapInsertBenchmark();
                }
                else if(strcasecmp(argv[i], "Arithmetic") == 0)
                {
                    arithmeticBenchmark();
                }
                else if(strcasecmp(argv[i], "StringMap") == 0)
                {
                    stringMapFindBenchmark();
                }
                else if(strcasecmp(argv[i], "Switch") == 0)
                {
                    switchBenchmark();
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

        mapFindBenchmark();
        mapInsertBenchmark();
        arithmeticBenchmark();
        stringMapFindBenchmark();
        switchBenchmark();
    }

    return 0;
}


