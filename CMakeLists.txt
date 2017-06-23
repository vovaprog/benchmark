cmake_minimum_required(VERSION 2.8)

project (benchmark)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(".")

set(SOURCE
    main.cpp
    ArithmeticBenchmark.h ArithmeticBenchmark.cpp
    MapBenchmark.h MapBenchmark.cpp
    IntrusiveMapBenchmark.h IntrusiveMapBenchmark.cpp
    StringMapBenchmark.h StringMapBenchmark.cpp
    SwitchBenchmark.h SwitchBenchmark.cpp
    Tools.h Tools.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -Wall -pedantic")

add_executable(benchmark ${SOURCE})

target_link_libraries(benchmark boost_filesystem boost_system)

add_custom_target(run_benchmark
    COMMAND "${CMAKE_BINARY_DIR}/benchmark" DEPENDS benchmark)

add_custom_target(generate_text COMMAND "bash" 
    "${CMAKE_SOURCE_DIR}/tools/fill_readme_template.sh" "${CMAKE_SOURCE_DIR}/tools" "${CMAKE_BINARY_DIR}/article"
    "${CMAKE_CXX_COMPILER}" "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" DEPENDS run_benchmark)
    
add_custom_target(generate_plots COMMAND "bash" 
    "${CMAKE_SOURCE_DIR}/tools/generate_plots.sh" "${CMAKE_BINARY_DIR}/plots_data" "${CMAKE_BINARY_DIR}/article/plots" "${CMAKE_SOURCE_DIR}/tools"
    DEPENDS run_benchmark)

add_custom_target(article DEPENDS generate_text generate_plots)

