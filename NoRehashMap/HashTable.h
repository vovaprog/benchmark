#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <boost/preprocessor.hpp>


template<typename K, typename T,
         typename HashType=boost::hash<K>,
         typename EqualType=std::equal_to<K>>
class HashTable {
public:
    typedef std::pair<K, T> value_type;

    HashTable():buckets(nullptr), _size(0)
    {
        reallocate(11);

        /*bucketCount = nextPrime(11);
        std::cout << "bucket count:" << bucketCount << std::endl;
        buckets = new Node*[bucketCount];
        memset(buckets, 0, bucketCount * sizeof(Node*));*/
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
        Node *next = 0;
        bool lastInBucket = false;
        //Node *nextIter;
    };


    class iterator {
    public:
        iterator(): table(nullptr), node(nullptr)
        {

        }

        iterator(HashTable<K, T, HashType, EqualType> *table, Node *node):
            table(table), node(node)
        {
        }

        value_type* operator->() const {
            return &node->value;
        }

        bool operator==(const iterator& iter) const {
            return (iter.table == table && iter.node == node) ||
                   (node == nullptr && iter.node == nullptr);
        }

        bool operator!=(const iterator& iter) const {
            return iter.table != table || iter.node != node;
        }

        iterator& operator++()
        {
            if (node != nullptr)
            {
                node = node->nextIter;
            }
            return *this;
        }

    //private:
        HashTable<K, T, HashType, EqualType> *table;
        //size_t bucketIndex;
        Node *node;
    };

    std::pair<iterator, bool> insertHashNoCheck(value_type &value, size_t hash)
    {
        Node * newNode = new Node();

        newNode->value = value;
        newNode->hash = hash;

        size_t bucketIndex = hash % bucketCount;
        Node *prev = buckets[bucketIndex];

        if (prev == nullptr)
        {
            if (beginNode.next != nullptr)
            {
                buckets[beginNode.next->hash % bucketCount] = newNode;
            }

            buckets[bucketIndex] = &beginNode;
            newNode->next = beginNode.next;
            beginNode.next = newNode;
            newNode->lastInBucket = true;
        }
        else
        {
            newNode->next = prev->next;
            prev->next = newNode;
            newNode->lastInBucket = false;
        }

        ++_size;

        return std::pair<iterator, bool>(iterator(this, newNode), true);
    }


    void link(Node *node)
    {
        size_t bucketIndex = node->hash % bucketCount;

        if(buckets[bucketIndex] == nullptr)
        {
            if (beginNode.next != nullptr)
            {
                buckets[beginNode.next->hash % bucketCount] = node;
            }
            buckets[bucketIndex] = &beginNode;
            node->next = beginNode.next;
            beginNode.next = node;
            node->lastInBucket = true;
        }
        else
        {
            node->next = buckets[bucketIndex]->next;
            buckets[bucketIndex]->next = node;
            node->lastInBucket = false;
        }

        ++_size;
    }

    void unlink(Node *node)
    {
        size_t bucketIndex = node->hash % bucketCount;

        Node *prev = buckets[bucketIndex];

        while (prev->next != node)
        {
            prev = prev->next;
        }

        prev->next = node->next;

        if (node->next != nullptr) {
            size_t nextBucketIndex = node->next->hash % bucketCount;

            if (nextBucketIndex != bucketIndex)
            {
                buckets[nextBucketIndex] = prev;
            }
        }

        assert(_size > 0);
        --_size;
    }

    void unlinkBegin()
    {
        assert(beginNode.next != nullptr);

        unlink(beginNode.next);

        //assert(beginNode.next != nullptr);

        /*assert(_begin.node != nullptr);

        iterator oldBegin = _begin;
        ++_begin;

        size_t bucketIndex = oldBegin.node->hash % bucketCount;
        Node *nodePtr = buckets[bucketIndex];

        if(nodePtr == oldBegin.node)
        {
            buckets[bucketIndex] = oldBegin.node->next;
        }
        else
        {
            while(nodePtr->next != oldBegin.node)
            {
                nodePtr = nodePtr->next;
            }

            nodePtr->next = oldBegin.node->next;
            nodePtr->nextIter = oldBegin.node->nextIter;
        }*/

        /*assert(_size > 0);
        --_size;*/
    }


    iterator findHash(const K& key, size_t hash)
    {
        size_t bucketIndex = hash % bucketCount;

        Node *nodePtr = buckets[bucketIndex];

        if (nodePtr == nullptr) {
            return end();
        }

        nodePtr = nodePtr->next;

        while(nodePtr != nullptr)
        {
            if (nodePtr->hash == hash && equalComparer(nodePtr->value.first, key))
            {
                return iterator(this, nodePtr);
            }

            if (nodePtr->lastInBucket == true)
            {
                break;
            }

            nodePtr = nodePtr->next;
        }

        return end();
    }

    iterator begin()
    {
        return iterator(this, beginNode.next);
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

    size_t max_size() const
    {
        return _maxSize;
    }

    void reallocate(size_t argBucketCount)
    {
        assert(_size == 0);

        bucketCount = nextPrime(argBucketCount);

        buckets = (decltype(buckets))realloc(buckets, bucketCount * sizeof(Node*));

        /*if (buckets != nullptr)
        {
            buckets = (decltype(buckets))realloc(buckets, bucketCount * sizeof(Node*));
        }
        else
        {
            std::cout << bucketCount * sizeof(Node*) << std::endl;
            buckets = (Node**)malloc(bucketCount * sizeof(Node*));
        }
        buckets[0] = (Node*)33;*/
        memset(buckets, 0, bucketCount * sizeof(Node*));

        _maxSize = bucketCount * max_load_factor();
    }

    static const size_t primes[];
    static const size_t primesCount;

    Node **buckets = nullptr;
    size_t bucketCount;

    HashType hashObject;
    EqualType equalObject;

    size_t _size;
    size_t _maxSize;


    iterator _begin;

    HashType hasher;
    EqualType equalComparer;

    Node beginNode;
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
