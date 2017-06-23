#include <Tools.h>

#include <set>
#include <fstream>
#include <boost/filesystem.hpp>

bool randomVector(std::vector<uint64_t> &keys, int64_t itemCount)
{
    keys.clear();

    std::set<uint64_t> s;

    for(int i = 0; i < itemCount; ++i)
    {
        int k;
        for(k = 0; k < 1000; ++k)
        {
            uint64_t key = randomUInt64();
            if(s.count(key) == 0)
            {
                keys.push_back(key);
                s.insert(key);
                break;
            }
        }
        if(k == 1000)
        {
            return false;
        }
    }
    if((int64_t)keys.size() != itemCount || (int64_t)s.size() != itemCount)
    {
        return false;
    }
    return true;
}


bool randomVector(std::vector<std::string> &keys, int64_t itemCount, int64_t keyLength)
{
    keys.clear();

    std::set<std::string> s;

    for(int i = 0; i < itemCount; ++i)
    {
        int k;
        for(k = 0; k < 1000; ++k)
        {
            std::string key = randomString(keyLength);
            if(s.count(key) == 0)
            {
                keys.push_back(key);
                s.insert(key);
                break;
            }
        }
        if(k == 1000)
        {
            return false;
        }
    }
    if((int64_t)keys.size() != itemCount || (int64_t)s.size() != itemCount)
    {
        return false;
    }
    return true;
}

bool resultToFile(const BenchmarkSingle &bench)
{
    std::string fileName("./plots_data");

    boost::filesystem::create_directory(fileName);

    for(auto &prefix : bench.prefixes)
    {
        fileName = fileName + "/" + prefix;
        boost::filesystem::create_directory(fileName);
    }

    fileName = fileName + "/" + bench.params.testName + ".txt";

    std::ofstream fl;
    fl.open(fileName);

    fl << bench.params.ticks << std::endl;

    return true;
}


bool resultToFile(const BenchmarkSet &benchSet)
{
    std::string fileName("./plots_data");

    boost::filesystem::create_directory(fileName);

    for(auto &prefix : benchSet.prefixes)
    {
        fileName = fileName + "/" + prefix;
        boost::filesystem::create_directory(fileName);
    }

    fileName = fileName + "/" + benchSet.params[0].testName + ".txt";

    std::ofstream fl;
    fl.open(fileName);

    for(const BenchmarkParameters &r : benchSet.params)
    {
        fl << r.itemCount << "|" << r.ticks << std::endl;
    }

    return true;
}


void randomString(char *s, size_t length)
{
    for(size_t i=0;i<length;++i)
    {
        s[i] = 'A' + rand() % 26;
    }
    s[length] = 0;
}


std::string randomString(size_t length)
{
    std::string s(length, ' ');

    for(size_t i=0;i<length;++i)
    {
        s[i] = 'A' + rand() % 26;
    }

    return s;
}

