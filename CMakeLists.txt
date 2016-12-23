cmake_minimum_required(VERSION 3.5)
project(cotests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(conn)
add_subdirectory(comm)

