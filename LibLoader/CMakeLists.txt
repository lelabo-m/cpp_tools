cmake_minimum_required(VERSION 2.8.4)
project(LibLoader)

set(Boost_INCLUDE_DIR /usr/local/boost_1_56_0)
set(Boost_LIBRARY_DIR /usr/local/boost_1_56_0/stage/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

set(SOURCE_FILES
    LibLoader/LibLoader.cpp
    LibLoader/LibLoader.hh
    LibLoader/LibManager.cpp
    LibLoader/LibManager.hh
    LibLoader/TestMain.cpp)

add_executable(LibLoader ${SOURCE_FILES})
target_link_libraries( LibLoader ${Boost_LIBRARIES} -ldl )