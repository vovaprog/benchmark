cmake_minimum_required(VERSION 2.8)

project (prog)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories("." "..")

set(SOURCE
    NoRehashMap.h
    NoRehashTable.h
    BlockStorage.h
    BlockStorageAllocator.h
    MallocAllocator.h
    NoRehashMapTest.cpp
    ../Tools.h ../Tools.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic-errors -fstack-protector")

add_executable(prog ${SOURCE})

target_link_libraries(prog boost_filesystem boost_system)
