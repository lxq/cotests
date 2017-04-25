cmake_minimum_required(VERSION 3.5)
project(comm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    command.cpp)

include_directories(
        $ENV{EQ_ROOT}/Lunchbox
        $ENV{EQ_ROOT}/build/Lunchbox/include
        $ENV{EQ_ROOT}/Collage
        $ENV{EQ_ROOT}/build/Collage/include
)

link_directories(
        $ENV{EQ_ROOT}/build/lib
)

#include(CMakeToolsHelpers OPTIONAL)

add_executable(comm ${SOURCE_FILES})

target_link_libraries(comm
        pthread
        Servus
        Pression
        Lunchbox
        Collage)
