#include <ProducerConsumer/ProducerConsumerBenchmark.h>

#include <ProducerConsumer/Producer.h>
#include <ProducerConsumer/QueueBoostLockfree.h>

#include <iostream>

bool producerConsumerBenchmark()
{
    std::cout << "===== producer consumer benchmark =====" << std::endl;

    QueueBoostLockfree<uint64_t> q;
    Producer<QueueBoostLockfree<uint64_t>> prod(q);
    prod.start();
    prod.stop();

    return true;
}
