cmake_minimum_required(VERSION 2.8.4)
project(Thread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES TestMain.cpp ThreadPool.cpp)
add_executable(Thread ${SOURCE_FILES})
