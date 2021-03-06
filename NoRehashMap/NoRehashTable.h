#ifndef NO_REHASH_TABLE_H
#define NO_REHASH_TABLE_H

#include <boost/preprocessor.hpp>


template<typename K, typename T>
struct NoRehashTableNode
{
    typedef std::pair<K, T> value_type;

    NoRehashTableNode(const value_type &argValue, size_t argHash):
        value(argValue), hash(argHash)
    {
    }

    NoRehashTableNode<K, T> *next = nullptr;
    bool lastInBucket = true;

    value_type value;
    size_t hash;
};


// Node without value. Is used to store begin node data.
// Data members must be the same as first members of NoRehashTableNode.
template<typename K, typename T>
struct NoRehashTableEmptyNode
{
    NoRehashTableNode<K, T> *next = nullptr;
    bool lastInBucket = true;
};


template<typename K, typename T,
         typename HashAlgoType,
         typename EqualAlgoType,
         typename NodeAllocator,
         typename BucketAllocator>
class NoRehashTable
{
private:
    typedef NoRehashTableNode<K, T> Node;

public:
    typedef typename Node::value_type value_type;


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


        value_type& operator*() const
        {
            return node->value;
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
                node = node->next;
            }
            return *this;
        }

        Node *node;
    };


    NoRehashTable(NodeAllocator *argNodeAllocator, BucketAllocator *argBucketAllocator):
        buckets(nullptr),
        curSize(0),
        nodeAllocator(argNodeAllocator),
        bucketAllocator(argBucketAllocator)
    {
        reallocate(InitialBuckets);
    }


    NoRehashTable(const NoRehashTable &) = delete;
    NoRehashTable(NoRehashTable &&) = delete;
    NoRehashTable& operator=(const NoRehashTable &) = delete;
    NoRehashTable& operator=(NoRehashTable &&) = delete;


    ~NoRehashTable()
    {
        deleteAllNodes();

        if(buckets != nullptr)
        {
            bucketAllocator->deallocate(buckets, bucketCount);
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

            cur->~Node();
            nodeAllocator->deallocate(cur, 1);

            cur = next;
        }

        curSize = 0;
        beginNode.next = nullptr;
    }


    iterator insertHashNoCheck(const value_type &value, size_t hash)
    {
        Node *newNodeMemory = nodeAllocator->allocate(1);
        Node *newNode = new (newNodeMemory) Node(value, hash);

        linkNode(newNode);

        return iterator(newNode);
    }


    void link(iterator &iter)
    {
        linkNode(iter.node);
    }


    void unlinkBegin()
    {
        assert(beginNode.next != nullptr);

        unlinkNext(&beginNode);
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


    bool erase(const K &key, size_t hash)
    {
        size_t bucketIndex = hash % bucketCount;

        Node *prev = buckets[bucketIndex];

        if(prev == nullptr)
        {
            return false;
        }

        while(prev->next != nullptr)
        {
            Node *cur = prev->next;

            if(cur->hash == hash && equalAlgo(cur->value.first, key))
            {
                unlinkNext(prev);

                cur->~Node();
                nodeAllocator->deallocate(cur, 1);

                return true;
            }

            if(cur->lastInBucket == true)
            {
                break;
            }

            prev = prev->next;
        }

        return false;
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
        return static_cast<float>(curSize) / static_cast<float>(bucketCount);
    }


    size_t size() const
    {
        return curSize;
    }


    size_t max_size() const
    {
        return maxSize;
    }


    void reallocate(size_t argBucketCount)
    {
        assert(curSize == 0);

        if(buckets != nullptr)
        {
            bucketAllocator->deallocate(buckets, bucketCount);
        }

        bucketCount = nextPrime(argBucketCount);

        buckets = bucketAllocator->allocate(bucketCount);

        memset(buckets, 0, bucketCount * sizeof(Node*));

        maxSize = bucketCount * max_load_factor();
    }


private:

    void linkNode(Node *node)
    {
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

        ++curSize;
    }


    template<typename PrevType>
    void unlinkNext(PrevType *prev)
    {
        Node *del = prev->next;

        if(del == nullptr)
        {
            return;
        }

        prev->next = del->next;

        size_t bucketIndex = del->hash % bucketCount;

        if(del->lastInBucket)
        {
            prev->lastInBucket = true;

            if(del->next)
            {
                size_t nextBucketIndex = del->next->hash % bucketCount;
                buckets[nextBucketIndex] = reinterpret_cast<Node*>(prev);
            }

            if(buckets[bucketIndex] == reinterpret_cast<Node*>(prev))
            {
                buckets[bucketIndex] = nullptr;
            }
        }

        assert(curSize > 0);
        --curSize;
    }


    static size_t nextPrime(size_t num)
    {
        size_t const* const primeListBegin = primes;
        size_t const* const primeListEnd = primes + primesCount;
        size_t const* bound = std::lower_bound(primeListBegin, primeListEnd, num);
        if(bound == primeListEnd)
        {
            --bound;
        }
        return *bound;
    }


public:
    HashAlgoType hashAlgo;

private:
    static const size_t InitialBuckets = 32;

    static const size_t primes[];
    static const size_t primesCount;

    Node **buckets = nullptr;
    size_t bucketCount;

    size_t curSize;
    size_t maxSize;

    EqualAlgoType equalAlgo;

    NoRehashTableEmptyNode<K, T> beginNode;

    NodeAllocator *nodeAllocator;
    BucketAllocator *bucketAllocator;
};


#define NO_REHASH_TABLE_PRIMES \
    (17ul)(29ul)(37ul)(53ul)(67ul)(79ul) \
    (97ul)(131ul)(193ul)(257ul)(389ul)(521ul)(769ul) \
    (1031ul)(1543ul)(2053ul)(3079ul)(6151ul)(12289ul)(24593ul) \
    (49157ul)(98317ul)(196613ul)(393241ul)(786433ul) \
    (1572869ul)(3145739ul)(6291469ul)(12582917ul)(25165843ul) \
    (50331653ul)(100663319ul)(201326611ul)(402653189ul)(805306457ul) \
    (1610612741ul)(3221225473ul)(4294967291ul)


template<typename K, typename T, typename HashType, typename EqualType, typename NodeAllocator, typename BucketAllocator>
const size_t NoRehashTable<K, T, HashType, EqualType, NodeAllocator, BucketAllocator>::primes[] =
{
    BOOST_PP_SEQ_ENUM(NO_REHASH_TABLE_PRIMES)
};

template<typename K, typename T, typename HashType, typename EqualType, typename NodeAllocator, typename BucketAllocator>
const size_t NoRehashTable<K, T, HashType, EqualType, NodeAllocator, BucketAllocator>::primesCount = BOOST_PP_SEQ_SIZE(NO_REHASH_TABLE_PRIMES);

#undef NO_REHASH_TABLE_PRIMES


#endif
