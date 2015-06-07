cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(PrettyDD)

if(WIN32)
	if(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")
endif()

add_subdirectory(src)
add_subdirectory(test)
