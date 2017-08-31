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
        Block *cur = blocksHead;
        while(cur != nullptr)
        {
            Block *temp = cur->next;
            delete cur;
            cur = temp;
        }
        blocksHead = nullptr;
        freeListHead = nullptr;
    }

#define NODE_TO_ITEM(node) (reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(node) + offsetof(Node, buf)))
#define ITEM_TO_NODE(item) (reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(item) - offsetof(Node, buf)))

    T* allocate()
    {
        if(freeListHead == nullptr)
        {
            allocateBlock();
        }

        Node *result = freeListHead;
        freeListHead = result->next;

        return NODE_TO_ITEM(result);
    }


    inline void free(T *p)
    {
        Node *node = ITEM_TO_NODE(p);

        node->next = freeListHead;
        freeListHead = node;
    }

#undef NODE_TO_ITEM
#undef ITEM_TO_NODE


    void getStorageInfo(size_t &allocatedItems, size_t &freeItems) const
    {
        freeItems = 0;

        Node *curNode = freeListHead;

        while(curNode != nullptr)
        {
            ++freeItems;
            curNode = curNode->next;
        }

        size_t totalItems = 0;

        Block *curBlock = blocksHead;

        while(curBlock != nullptr)
        {
            totalItems += BlockSize;
            curBlock = curBlock->next;
        }

        allocatedItems = totalItems - freeItems;
    }


private:

    inline void allocateBlock()
    {
        Block *newBlock = new Block;
        newBlock->next = blocksHead;
        blocksHead = newBlock;

        newBlock->nodes[BlockSize - 1].next = nullptr;
        if(BlockSize > 1)
        {
            for(int i = 0; i < BlockSize - 1; ++i)
            {
                newBlock->nodes[i].next = &(newBlock->nodes[i + 1]);
            }
        }

        freeListHead = &(newBlock->nodes[0]);
    }


private:

    struct Node
    {
        char buf[sizeof(T)];
        Node *next = nullptr;
    };

    struct Block
    {
        Block *next = nullptr;
        Node nodes[BlockSize];
    };

    Block *blocksHead = nullptr;
    Node *freeListHead = nullptr;
};

#endif // BLOCK_STORAGE_H
