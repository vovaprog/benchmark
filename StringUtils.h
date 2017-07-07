#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string.h>
#include <boost/functional/hash.hpp>

struct CharPointerLess
{
    bool operator()(const char *a, const char *b) const
    {
        return strcmp(a, b) < 0;
    }
};

struct CharPointerEqual
{
    bool operator()(const char *a, const char *b) const
    {
        return strcmp(a, b) == 0;
    }
};


struct CharPointerHashCombine
{
    size_t operator()(const char *a) const
    {
        std::size_t seed = 0;

        for(size_t i = 0; a[i] != 0; ++i)
        {
            boost::hash_combine(seed, a[i]);
        }

        return seed;
    }
};


struct CharPointerHashFnv1
{
    static const uint64_t fnvOffsetBasis = 0xcbf29ce484222325ULL;
    static const uint64_t fnvPrime = 0x100000001b3ULL;

    BOOST_STATIC_ASSERT(std::is_same<size_t, uint64_t>::value);
    BOOST_STATIC_ASSERT(sizeof(char) == 1);

    size_t operator()(const char *a) const
    {
        uint64_t hash = fnvOffsetBasis;

        for (; *a != 0; ++a)
        {
            hash *= fnvPrime;
            hash ^= static_cast<uint8_t>(*a);
        }

        return static_cast<size_t>(hash);
    }
};


struct CharPointerHashMurmur
{
    inline std::size_t load_bytes(const char* p, int n) const
    {
        std::size_t result = 0;
        --n;
        do
            result = (result << 8) + static_cast<unsigned char>(p[n]);
        while (--n >= 0);
        return result;
    }

    BOOST_STATIC_ASSERT(sizeof(size_t) == 8);

    inline std::size_t shift_mix(std::size_t v) const
    {
        return v ^ (v >> 47);
    }

    inline std::size_t unaligned_load(const char* p) const
    {
        std::size_t result;
        __builtin_memcpy(&result, p, sizeof(result));
        return result;
    }

    // Implementation of Murmur hash for 64-bit size_t.
    inline size_t murmurHash64(const void* ptr, size_t len, size_t seed) const
    {
        static const size_t mul = (((size_t) 0xc6a4a793UL) << 32UL)
                    + (size_t) 0x5bd1e995UL;
        const char* const buf = static_cast<const char*>(ptr);

        // Remove the bytes not divisible by the sizeof(size_t).  This
        // allows the main loop to process the data as 64-bit integers.
        const int len_aligned = len & ~0x7;
        const char* const end = buf + len_aligned;
        size_t hash = seed ^ (len * mul);
        for (const char* p = buf; p != end; p += 8)
        {
            const size_t data = shift_mix(unaligned_load(p) * mul) * mul;
            hash ^= data;
            hash *= mul;
        }
        if ((len & 0x7) != 0)
        {
            const size_t data = load_bytes(end, len & 0x7);
            hash ^= data;
            hash *= mul;
        }
        hash = shift_mix(hash) * mul;
        hash = shift_mix(hash);
        return hash;
    }

    size_t operator()(const char *a) const
    {
        return murmurHash64(a, strlen(a), 0);
    }
};


#endif

