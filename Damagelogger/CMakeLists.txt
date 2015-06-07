cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(PrettyDD)
#set(PrettyDD_VERSION_MAJOR 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(PrettyDD main.cpp player.cpp parse.cpp)
