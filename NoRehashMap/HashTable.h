#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <boost/preprocessor.hpp>


template<typename K, typename T,
         typename HashType=boost::hash<K>,
         typename EqualType=std::equal_to<K>>
class HashTable {
public:
    typedef std::pair<K, T> value_type;

    HashTable()
    {
        bucketCount = nextPrime(11);
        std::cout << "bucket count:" << bucketCount << std::endl;
        buckets = new Node*[bucketCount];
        memset(buckets, 0, bucketCount * sizeof(Node*));
    }

    static size_t nextPrime(size_t num)
    {
        size_t const* const prime_list_begin = primes;
        size_t const* const prime_list_end = primes + primesCount;
        size_t const* bound =
            std::lower_bound(prime_list_begin, prime_list_end, num);
        if(bound == prime_list_end)
            bound--;
        return *bound;
    }

    struct Node {
        value_type value;
        size_t hash;
        Node *next;
    };


    class iterator {
    public:
        iterator(): table(nullptr), bucketIndex(0), node(nullptr)
        {

        }

        iterator(HashTable<K, T> *table, size_t bucketIndex, Node *node):
            table(table), bucketIndex(bucketIndex), node(node)
        {
        }

        value_type* operator->() const {
            return &node->value;
        }

        bool operator==(const iterator& iter) const {
            return (iter.table == table && iter.bucketIndex == bucketIndex && iter.node == node) ||
                   (node == nullptr && iter.node == nullptr);
        }

        bool operator!=(const iterator& iter) const {
            return iter.table != table || iter.bucketIndex != bucketIndex || iter.node != node;
        }

        iterator& operator++()
        {
            if(node->next != nullptr)
            {
                node = node->next;
                return *this;
            }
            for(;bucketIndex < table->bucketCount; ++bucketIndex)
            {
                if(table->buckets[bucketIndex] != nullptr)
                {
                    node = table->buckets[bucketIndex];
                    return *this;
                }
            }

            node = nullptr;
            return *this;
        }

    //private:
        HashTable<K, T> *table;
        size_t bucketIndex;
        Node *node;
    };

    std::pair<iterator, bool> insert(value_type &value)
    {
        size_t hash = hashObject(value.first);
        size_t bucketIndex = hash % bucketCount;

        Node *nodePtr = buckets[bucketIndex];

        while(nodePtr != nullptr)
        {
            if (equalObject(nodePtr->value.first, value.first))
            {
                return std::pair<iterator, bool>(end(), false);
            }
            nodePtr = nodePtr->next;
        }

        nodePtr = new Node();
        nodePtr->value = value;
        nodePtr->hash = hash;
        nodePtr->next = buckets[bucketIndex];
        buckets[bucketIndex] = nodePtr;

        ++_size;

        return std::pair<iterator, bool>(iterator(this, bucketIndex, nodePtr), true);
    }


    void link(Node *node)
    {
        size_t bucketIndex = node->hash % bucketCount;

        node->next = buckets[bucketIndex];
        buckets[bucketIndex] = node;
    }

    void unlinkBegin()
    {
        iterator oldBegin = _begin;
        ++_begin;

        Node *nodePtr = buckets[oldBegin.bucketIndex];

        if(nodePtr == oldBegin.node)
        {
            buckets[oldBegin.bucketIndex] = oldBegin.node->next;
        }
        else
        {
            while(nodePtr->next != oldBegin.node)
            {
                nodePtr = nodePtr->next;
            }

            nodePtr->next = oldBegin.node->next;
        }
    }


    iterator findHash(const K& key, size_t hash)
    {
        size_t bucketIndex = hash % bucketCount;

        Node *nodePtr = buckets[bucketIndex];

        while(nodePtr != nullptr)
        {
            if (nodePtr->hash == hash && equalComparer(nodePtr->value.first, key))
            {
                return iterator(this, bucketIndex, nodePtr);
            }
            nodePtr = nodePtr->next;
        }

        return end();
    }

    iterator begin() const
    {
        return _begin;
    }

    iterator end() const
    {
        return iterator();
    }

    float max_load_factor() const
    {
        return 1.0;
    }

    float load_factor() const
    {
        return static_cast<float>(_size) / static_cast<float>(bucketCount);
    }

    size_t size() const
    {
        return _size;
    }

    void reallocate(size_t newBucketCount)
    {
        assert(_size == 0);
        buckets = (decltype(buckets))realloc(buckets, newBucketCount * sizeof(Node*));
        memset(buckets, 0, newBucketCount * sizeof(Node*));
        bucketCount = newBucketCount;
    }

    static const size_t primes[];
    static const size_t primesCount;

    Node **buckets;
    size_t bucketCount;

    HashType hashObject;
    EqualType equalObject;

    size_t _size;


    iterator _begin;

    HashType hasher;
    EqualType equalComparer;
};

#define HASH_TABLE_PRIMES \
    (17ul)(29ul)(37ul)(53ul)(67ul)(79ul) \
    (97ul)(131ul)(193ul)(257ul)(389ul)(521ul)(769ul) \
    (1031ul)(1543ul)(2053ul)(3079ul)(6151ul)(12289ul)(24593ul) \
    (49157ul)(98317ul)(196613ul)(393241ul)(786433ul) \
    (1572869ul)(3145739ul)(6291469ul)(12582917ul)(25165843ul) \
    (50331653ul)(100663319ul)(201326611ul)(402653189ul)(805306457ul) \
    (1610612741ul)(3221225473ul)(4294967291ul)


template<typename K, typename T, typename HashType, typename EqualType>
const size_t HashTable<K, T, HashType, EqualType>::primes[] = {
    BOOST_PP_SEQ_ENUM(HASH_TABLE_PRIMES)
};

template<typename K, typename T, typename HashType, typename EqualType>
const size_t HashTable<K, T, HashType, EqualType>::primesCount = BOOST_PP_SEQ_SIZE(HASH_TABLE_PRIMES);

#endif
