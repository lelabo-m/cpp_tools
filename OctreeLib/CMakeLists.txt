cmake_minimum_required(VERSION 2.8.4)
project(OctreeLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    Tree.cpp
    Node.cpp
    Octree.hpp)

add_executable(OctreeLib ${SOURCE_FILES})