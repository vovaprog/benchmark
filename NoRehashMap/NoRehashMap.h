#ifndef INCREMENTA_HASH_MAP_H
#define INCREMENTA_HASH_MAP_H

#include <boost/functional/hash.hpp>

#include <HashTable.h>


template<typename K, typename T,
         typename HashAlgoType=boost::hash<K>,
         typename EqualType=std::equal_to<K>,
         int movePerInsert=1>
class NoRehashMap {
public:
    typedef typename HashTable<K, T, HashAlgoType, EqualType>::iterator iterator;
    typedef typename HashTable<K, T, HashAlgoType, EqualType>::value_type value_type;


    NoRehashMap():
        insertTable0(&table0), insertTable1(&table1),
        findTable0(&table0), findTable1(&table1)
    {
    }


    std::pair<iterator, bool> insert(value_type &value)
    {
        size_t hash = table0.hasher(value.first);

        iterator iter = findHash(value.first, hash);
        if (iter != table0.end())
        {
            return std::pair<iterator, bool>(iter, false);
        }

        if (insertTable0->size() >= insertTable0->max_size())
        {
            assert(insertTable1->size() == 0);

            insertTable1->reallocate(insertTable0->size() * 2);

            std::swap(insertTable0, insertTable1);
        }

        for (int i = 0; i < movePerInsert; ++i)
        {
            iterator iter = insertTable1->begin();
            if (iter == insertTable1->end())
            {
                break;
            }
            insertTable1->unlinkBegin();
            insertTable0->link(iter.node);
        }

        if (insertTable0->size() >= insertTable1->size() && findTable0 == insertTable1)
        {
            std::swap(findTable0, findTable1);
        }

        return insertTable0->insertHashNoCheck(value, hash);
    }

    iterator findHash(const K &key, size_t hash)
    {
        iterator iter = findTable0->findHash(key, hash);
        if(iter != findTable0->end())
        {
            return iter;
        }
        iter = findTable1->findHash(key, hash);
        return iter;
    }

    iterator find(const K &key)
    {
        size_t hash = table0.hasher(key);
        return findHash(key, hash);
    }

    HashTable<K, T, HashAlgoType, EqualType> table0, table1;
    HashTable<K, T, HashAlgoType, EqualType> *insertTable0, *insertTable1;
    HashTable<K, T, HashAlgoType, EqualType> *findTable0, *findTable1;
};


#endif

