import random
import timeit
import time


def getTicks():
    return int(time.time() * 1000000)


def benchmarkMapFind(itemCount, iterCount):
    keys = []
    m = {}
    for i in range(itemCount):
        r = random.getrandbits(64)
        while r in m:
            r = random.getrandbits(64)
        keys.append(r)
        m[r] = 33

    counter = 0

    ticks = getTicks()

    while counter < iterCount:

        for i in range(itemCount):
            val = m[keys[i]]
            if val != 33:
                return False
            counter += 1
            if counter == iterCount:
                break

    ticks = getTicks() - ticks

    print str(itemCount) + "|" + str(ticks)


for itemCount in range(5, 350, 1):
    benchmarkMapFind(itemCount, 100000)
