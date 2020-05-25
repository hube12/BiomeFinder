cmake_minimum_required(VERSION 3.10)
project(BiomeFinder)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -static -O3")

add_subdirectory(src)