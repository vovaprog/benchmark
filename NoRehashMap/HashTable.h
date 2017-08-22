#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <boost/preprocessor.hpp>


template<typename K, typename T,
         typename HashAlgoType = boost::hash<K>,
         typename EqualAlgoType = std::equal_to<K>,
         typename NodeAllocator = std::allocator<std::pair<const K, T>>,
         typename BucketAllocator = std::allocator<void*>>
class HashTable
{
public:

    typedef std::pair<K, T> value_type;

private:

    struct Node
    {
        Node *next = nullptr;
        value_type value;
        size_t hash;
        bool lastInBucket = false;
    };


    struct EmptyNode
    {
        Node *next = nullptr;
    };


    static const size_t InitialBuckets = 32;

public:

    class iterator
    {
    public:
        iterator(): node(nullptr)
        {
        }


        explicit iterator(Node *node): node(node)
        {
        }


        value_type* operator->() const
        {
            return &node->value;
        }


        bool operator==(const iterator& iter) const
        {
            return node == iter.node;
        }


        bool operator!=(const iterator& iter) const
        {
            return node != iter.node;
        }


        iterator& operator++()
        {
            if(node != nullptr)
            {
                node = node->nextIter;
            }
            return *this;
        }

    private:

        Node *node;

        friend class HashTable;
    };


    HashTable(): buckets(nullptr), _size(0)
    {
        reallocate(InitialBuckets);
    }


    ~HashTable()
    {
        deleteAllNodes();

        if(buckets != nullptr)
        {
            bucketAllocator.deallocate(buckets, bucketCount);
        }
    }


    void clear()
    {
        deleteAllNodes();
        reallocate(InitialBuckets);
    }


    void deleteAllNodes()
    {
        Node *cur = beginNode.next;

        while(cur != nullptr)
        {
            Node *next = cur->next;
            nodeAllocator.deallocate(cur, 1);
            cur = next;
        }

        _size = 0;
        beginNode.next = nullptr;
    }


    iterator insertHashNoCheck(const value_type &value, size_t hash)
    {
        Node *newNodeMemory = nodeAllocator.allocate(1);
        Node *newNode = new (newNodeMemory) Node;

        newNode->value = value;
        newNode->hash = hash;

        size_t bucketIndex = hash % bucketCount;
        Node *prev = buckets[bucketIndex];

        if(prev == nullptr)
        {
            if(beginNode.next != nullptr)
            {
                buckets[beginNode.next->hash % bucketCount] = newNode;
            }

            buckets[bucketIndex] = reinterpret_cast<Node*>(&beginNode);
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

        return iterator(newNode);
    }


    void link(iterator &iter)
    {
        Node *node = iter.node;

        size_t bucketIndex = node->hash % bucketCount;

        if(buckets[bucketIndex] == nullptr)
        {
            if(beginNode.next != nullptr)
            {
                buckets[beginNode.next->hash % bucketCount] = node;
            }
            buckets[bucketIndex] = reinterpret_cast<Node*>(&beginNode);
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

        while(prev->next != node)
        {
            prev = prev->next;
        }

        prev->next = node->next;

        // if next node is in another bucket - change pointer in buckets array.
        if(node->next != nullptr)
        {
            size_t nextBucketIndex = node->next->hash % bucketCount;

            if(nextBucketIndex != bucketIndex)
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
    }


    iterator findHash(const K& key, size_t hash)
    {
        size_t bucketIndex = hash % bucketCount;

        Node *nodePtr = buckets[bucketIndex];

        if(nodePtr == nullptr)
        {
            return end();
        }

        nodePtr = nodePtr->next;

        while(nodePtr != nullptr)
        {
            if(nodePtr->hash == hash && equalAlgo(nodePtr->value.first, key))
            {
                return iterator(nodePtr);
            }

            if(nodePtr->lastInBucket == true)
            {
                break;
            }

            nodePtr = nodePtr->next;
        }

        return end();
    }


    iterator begin() const
    {
        return iterator(beginNode.next);
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

        if(buckets != nullptr)
        {
            bucketAllocator.deallocate(buckets, bucketCount);
        }

        bucketCount = nextPrime(argBucketCount);

        buckets = bucketAllocator.allocate(bucketCount);

        memset(buckets, 0, bucketCount * sizeof(Node*));

        _maxSize = bucketCount * max_load_factor();
    }

    HashAlgoType hashAlgo;


private:

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


    static const size_t primes[];
    static const size_t primesCount;

    Node **buckets = nullptr;
    size_t bucketCount;

    HashAlgoType hashObject;
    EqualAlgoType equalObject;

    size_t _size;
    size_t _maxSize;

    EqualAlgoType equalAlgo;

    EmptyNode beginNode;

    typename NodeAllocator::template rebind<Node>::other nodeAllocator;
    typename BucketAllocator::template rebind<Node*>::other bucketAllocator;
};


#define HASH_TABLE_PRIMES \
    (17ul)(29ul)(37ul)(53ul)(67ul)(79ul) \
    (97ul)(131ul)(193ul)(257ul)(389ul)(521ul)(769ul) \
    (1031ul)(1543ul)(2053ul)(3079ul)(6151ul)(12289ul)(24593ul) \
    (49157ul)(98317ul)(196613ul)(393241ul)(786433ul) \
    (1572869ul)(3145739ul)(6291469ul)(12582917ul)(25165843ul) \
    (50331653ul)(100663319ul)(201326611ul)(402653189ul)(805306457ul) \
    (1610612741ul)(3221225473ul)(4294967291ul)


template<typename K, typename T, typename HashType, typename EqualType, typename NodeAllocator, typename BucketAllocator>
const size_t HashTable<K, T, HashType, EqualType, NodeAllocator, BucketAllocator>::primes[] =
{
    BOOST_PP_SEQ_ENUM(HASH_TABLE_PRIMES)
};

template<typename K, typename T, typename HashType, typename EqualType, typename NodeAllocator, typename BucketAllocator>
const size_t HashTable<K, T, HashType, EqualType, NodeAllocator, BucketAllocator>::primesCount = BOOST_PP_SEQ_SIZE(HASH_TABLE_PRIMES);

#endif
