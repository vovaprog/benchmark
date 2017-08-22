cmake_minimum_required(VERSION 2.8)

project (prog)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(".")

set(SOURCE
    NoRehashMap.h
    HashTable.h
    BlockStorage.h
    BlockStorageAllocator.h
    MallocAllocator.h
    NoRehashMapTest.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++11 -Wall -pedantic -pthread")

add_executable(prog ${SOURCE})


