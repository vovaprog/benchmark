#ifndef PRODUCER_H_INCLUDED
#define PRODUCER_H_INCLUDED

#include <thread>
#include <atomic>

template<typename QueueType>
class Producer {
public:
    Producer(QueueType &q):q(q)
    {
    }

    void produce()
    {
        ++counter;
        while(!q.push(counter)) {}
    }

    void start()
    {
        runFlag.store(true);
        th = new std::thread(&Producer::threadEntry, this);
    }

    void stop()
    {
        if(th != nullptr)
        {
            runFlag.store(false);
            th->join();
            delete th;
            th = nullptr;
        }
    }

    void threadEntry()
    {
        while(runFlag.load())
        {
            produce();
        }
    }

private:
    QueueType &q;
    uint64_t counter = 0;
    std::atomic_bool runFlag;
    std::thread *th;
};

#endif

