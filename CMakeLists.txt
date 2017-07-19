cmake_minimum_required(VERSION 2.8)

project (benchmark)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(".")

set(SOURCE
    main.cpp
    AlignBenchmark.h AlignBenchmark.cpp
    ArithmeticBenchmark.h ArithmeticBenchmark.cpp
    MapBenchmark.h MapBenchmark.cpp
    IntrusiveMapBenchmark.h IntrusiveMapBenchmark.cpp
    QueueBenchmark.h QueueBenchmark.cpp
    StringMapBenchmark.h StringMapBenchmark.cpp
    StringUtils.h
    SwitchBenchmark.h SwitchBenchmark.cpp
    Tools.h Tools.cpp
    ProducerConsumer/ProducerConsumerBenchmark.h ProducerConsumer/ProducerConsumerBenchmark.cpp
    ProducerConsumer/Producer.h ProducerConsumer/Consumer.h ProducerConsumer/QueueBoostLockfree.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -pthread -Wall -pedantic")

add_executable(benchmark ${SOURCE})

target_link_libraries(benchmark boost_filesystem boost_system tbb)


add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/plots_data" COMMAND "${CMAKE_BINARY_DIR}/benchmark" DEPENDS benchmark)
add_custom_target(run_benchmark DEPENDS "${CMAKE_BINARY_DIR}/plots_data")


add_custom_target(generate_pages COMMAND "bash" 
    "${CMAKE_SOURCE_DIR}/site/generate_site.sh" "${CMAKE_SOURCE_DIR}/site" "${CMAKE_BINARY_DIR}/site"
    "${CMAKE_CXX_COMPILER}" "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" DEPENDS run_benchmark)
add_custom_target(generate_pages_nodeps COMMAND "bash" 
    "${CMAKE_SOURCE_DIR}/site/generate_site.sh" "${CMAKE_SOURCE_DIR}/site" "${CMAKE_BINARY_DIR}/site"
    "${CMAKE_CXX_COMPILER}" "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")


add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/site/plots" COMMAND "bash" 
    "${CMAKE_SOURCE_DIR}/tools/generate_plots.sh" "${CMAKE_BINARY_DIR}/plots_data" 
    "${CMAKE_BINARY_DIR}/site/plots" "${CMAKE_SOURCE_DIR}/tools")
add_custom_target(generate_plots DEPENDS run_benchmark "${CMAKE_BINARY_DIR}/site/plots")
add_custom_target(generate_plots_nodeps DEPENDS "${CMAKE_BINARY_DIR}/site/plots")


add_custom_target(site DEPENDS generate_pages generate_plots)

