#include <IntrusiveMapBenchmark.h>

#include <vector>
#include <boost/intrusive/set.hpp>
#include <boost/intrusive/avl_set.hpp>

#include <Tools.h>

namespace
{

struct DataIntMap: public boost::intrusive::set_base_hook<boost::intrusive::optimize_size<true>>
{
    uint64_t key = 0;
    uint64_t a = 0, b = 0, c = 0, d = 0;

    bool operator<(const DataIntMap &d) const
    {
        return key < d.key;
    }
};

struct DataAvlSet: public boost::intrusive::avl_set_base_hook<boost::intrusive::optimize_size<true>>
{
    uint64_t key = 0;
    uint64_t a = 0, b = 0, c = 0, d = 0;

    bool operator<(const DataAvlSet &d) const
    {
        return key < d.key;
    }
};


template<typename DataType>
struct DataCompare
{
   bool operator()(uint64_t key, const DataType &d) const
   {
       return key < d.key;
   }

   bool operator()(const DataType &d, uint64_t key) const
   {
       return d.key < key;
   }
};

}


template<typename MapType, typename DataType, typename CompareType>
bool benchIntrusive(BenchmarkParameters &params)
{
    std::vector<uint64_t> &keys = *reinterpret_cast<std::vector<uint64_t>*>(params.arg0);

    std::vector<DataType> values;
    values.reserve(params.itemCount);

    for(int i = 0; i < params.itemCount; ++i)
    {
        DataType d;
        d.key = keys[i];
        values.push_back(d);
    }

    {
        MapType m;

        for(DataType &d : values)
        {
            m.insert(d);
        }

        if(m.size() != static_cast<uint64_t>(params.itemCount))
        {
            return false;
        }

        int64_t searchCounter = 0;

        uint64_t ticks = getTicks();

        //=======================================================================

        while(searchCounter < params.iterCount)
        {
            for(int i = 0; i < params.itemCount && searchCounter < params.iterCount; ++i, ++searchCounter)
            {
                auto iter = m.find(keys[i], CompareType());

                if(iter == m.end() || iter->key != keys[i])
                {
                    return false;
                }
            }
        }

        //=======================================================================

        params.ticks = getTicks() - ticks;
    }

    return true;
}


bool intrusiveSetFindBenchmark(BenchmarkParameters &params)
{
    params.testName = "boost intrusive set";
    return benchIntrusive<boost::intrusive::set<DataIntMap>, DataIntMap, DataCompare<DataIntMap>>(params);
}


bool intrusiveAvlSetFindBenchmark(BenchmarkParameters &params)
{
    params.testName = "boost intrusive avl_set";
    return benchIntrusive<boost::intrusive::avl_set<DataAvlSet>, DataAvlSet, DataCompare<DataAvlSet>>(params);
}

