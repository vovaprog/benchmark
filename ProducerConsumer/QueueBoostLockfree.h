#ifndef PRODUCER_CONSUMER_QUEUE_H
#define PRODUCER_CONSUMER_QUEUE_H

#include <boost/lockfree/queue.hpp>

template<typename T>
class QueueBoostLockfree {
public:
    bool push(const T &t)
    {
        return q.push(t);
    }

    bool pop(T &t)
    {
        return q.pop(t);
    }

private:
    boost::lockfree::queue<T> q;
};


#endif

