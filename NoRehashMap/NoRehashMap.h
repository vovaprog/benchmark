#ifndef INCREMENTA_HASH_MAP_H
#define INCREMENTA_HASH_MAP_H

#include <boost/functional/hash.hpp>

#include <NoRehashTable.h>


template<typename K, typename T,
         typename HashAlgoType = boost::hash<K>,
         typename EqualType = std::equal_to<K>,
         typename NodeAllocator = std::allocator<std::pair<const K, T>>,
         typename BucketAllocator = std::allocator<void*>,
         int MovePerInsert = 1>
class NoRehashMap
{
private:

    typedef NoRehashTableNode<K, T> Node;

    typedef typename NodeAllocator::template rebind<Node>::other NodeAllocatorType;
    typedef typename BucketAllocator::template rebind<Node*>::other BucketAllocatorType;

    typedef NoRehashTable<K, T, HashAlgoType, EqualType, NodeAllocatorType, BucketAllocatorType> HashTableType;

public:

    typedef typename HashTableType::value_type value_type;

    class iterator
    {
    public:
        iterator(): nextTable(nullptr)
        {
        }

        iterator(const typename HashTableType::iterator &iter, HashTableType *nextTable):
            tableIter(iter), nextTable(nextTable)
        {
        }


        iterator& operator++()
        {
            ++tableIter;
            if(tableIter.node == nullptr)
            {
                if(nextTable != nullptr)
                {
                    tableIter = nextTable->begin();
                    nextTable = nullptr;
                }
            }
            return *this;
        }


        value_type* operator->() const
        {
            return tableIter.operator->();
        }


        value_type& operator*() const
        {
            return tableIter.operator*();
        }


        bool operator==(const iterator& iter) const
        {
            return this->tableIter == iter.tableIter;
        }


        bool operator!=(const iterator& iter) const
        {
            return this->tableIter != iter.tableIter;
        }

    private:

        typename HashTableType::iterator tableIter;
        HashTableType *nextTable;
    };


    NoRehashMap():
        table0(&nodeAllocator, &bucketAllocator),
        table1(&nodeAllocator, &bucketAllocator),
        insertTable0(&table0), insertTable1(&table1),
        findTable0(&table0), findTable1(&table1)
    {
    }


    NoRehashMap(const NoRehashMap &) = delete;
    NoRehashMap(NoRehashMap &&) = delete;
    NoRehashMap& operator=(const NoRehashMap &) = delete;
    NoRehashMap& operator=(NoRehashMap &&) = delete;


    std::pair<iterator, bool> insert(const value_type &value)
    {
        size_t hash = table0.hashAlgo(value.first);

        iterator iter = findHash(value.first, hash);
        if(iter != end())
        {
            return std::pair<iterator, bool>(iter, false);
        }

        return insertNoCheck(value, hash);
    }


    iterator find(const K &key)
    {
        size_t hash = table0.hashAlgo(key);
        return findHash(key, hash);
    }


    T& operator[](const K &key)
    {
        size_t hash = table0.hashAlgo(key);

        iterator iter = findHash(key, hash);
        if(iter != end())
        {
            return iter->second;
        }

        std::pair<iterator, bool> p = insertNoCheck(value_type(key, T()), hash);

        return p.first->second;
    }


    size_t erase(const K& key)
    {
        size_t hash = table0.hashAlgo(key);

        if(findTable0->erase(key, hash))
        {
            return 1;
        }

        if(findTable1->erase(key, hash))
        {
            return 1;
        }

        return 0;
    }


    size_t size() const
    {
        return table0.size() + table1.size();
    }


    iterator begin() const
    {
        return iterator(findTable0->begin(), findTable1);
    }


    iterator end() const
    {
        return iterator(table0.end(), nullptr);
    }


    void clear()
    {
        table0.clear();
        table1.clear();
    }

private:

    std::pair<iterator, bool> insertNoCheck(const value_type &value, size_t hash)
    {
        if(insertTable0->size() >= insertTable0->max_size())
        {
            assert(insertTable1->size() == 0);

            insertTable1->reallocate(insertTable0->size() * 2);

            std::swap(insertTable0, insertTable1);
        }

        for(int i = 0; i < MovePerInsert; ++i)
        {
            typename HashTableType::iterator iter = insertTable1->begin();
            if(iter == insertTable1->end())
            {
                break;
            }
            insertTable1->unlinkBegin();
            insertTable0->link(iter);
        }

        if(insertTable0->size() >= insertTable1->size() && findTable0 == insertTable1)
        {
            std::swap(findTable0, findTable1);
        }

        typename HashTableType::iterator tabIter = insertTable0->insertHashNoCheck(value, hash);
        HashTableType *nextTable = insertTable0 == findTable0 ? findTable1 : nullptr;
        return std::make_pair(iterator(tabIter, nextTable), true);
    }


    iterator findHash(const K &key, size_t hash)
    {
        typename HashTableType::iterator iter = findTable0->findHash(key, hash);
        if(iter != findTable0->end())
        {
            return iterator(iter, findTable1);
        }
        iter = findTable1->findHash(key, hash);
        return iterator(iter, nullptr);
    }


private:

    // Allocators must be declared before tables.
    // They must be created before tables and destroyed after tables.
    NodeAllocatorType nodeAllocator;
    BucketAllocatorType bucketAllocator;

    HashTableType table0, table1;

    HashTableType *insertTable0, *insertTable1;
    HashTableType *findTable0, *findTable1;
};


#endif

