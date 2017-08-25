#ifndef BLOCK_STORAGE_H
#define BLOCK_STORAGE_H

template <typename T, int BlockSize = 0x400>
class BlockStorage
{
public:

    BlockStorage() = default;

    BlockStorage(const BlockStorage &) = delete;
    BlockStorage(BlockStorage &&) = delete;
    BlockStorage& operator=(const BlockStorage &) = delete;
    BlockStorage& operator=(BlockStorage &&) = delete;

    ~BlockStorage()
    {
        destroy();
    }


    void destroy()
    {
        Block *cur = blocks;
        while(cur != nullptr)
        {
            Block *temp = cur->next;
            delete cur;
            cur = temp;
        }
        blocks = nullptr;
        freeListHead = nullptr;
    }

#define NODE_TO_ITEM(node) (reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(node) + offsetof(Node, buf)))
#define ITEM_TO_NODE(p) (reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(p) - offsetof(Node, buf)))

    T* allocate()
    {
        if(freeListHead == nullptr)
        {
            allocateBlock();
        }

        Node *result = freeListHead;
        freeListHead = result->blockStorageData.next;

        return NODE_TO_ITEM(result);
    }


    inline void free(T *p)
    {
        Node *node = ITEM_TO_NODE(p);

        node->blockStorageData.next = freeListHead;
        freeListHead = node;
    }

#undef NODE_TO_ITEM
#undef ITEM_TO_NODE

private:

    inline void allocateBlock()
    {
        Block *newBlock = new Block;
        newBlock->next = blocks;
        blocks = newBlock;

        newBlock->nodes[BlockSize - 1].blockStorageData.next = nullptr;
        if(BlockSize > 1)
        {
            for(int i = 0; i < BlockSize - 1; ++i)
            {
                newBlock->nodes[i].blockStorageData.next = &(newBlock->nodes[i + 1]);
            }
        }

        freeListHead = &(newBlock->nodes[0]);
    }


private:

    struct Node;
    struct ServiceData
    {
        Node *next = nullptr;
    };

    struct Node
    {
        char buf[sizeof(T)];
        ServiceData blockStorageData;
    };

    struct Block
    {
        Block *next = nullptr;
        Node nodes[BlockSize];
    };

    Block *blocks = nullptr;
    Node *freeListHead = nullptr;
};

#endif // BLOCK_STORAGE_H
