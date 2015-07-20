cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(PrettyDD CXX C)

find_package(Threads REQUIRED)

if(WIN32)
	if(MSVC)
		set(flags "${CMAKE_CXX_FLAGS} /W4 /WX /EHsc")
	endif()
else()
	set(flags "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
endif()
if (NOT CONFIGURED_ONCE)
	set(CMAKE_CXX_FLAGS "${flags}"
	    CACHE STRING "Flags used by the compiler during all build types." FORCE)
	set(CMAKE_C_FLAGS   "${flags}"
	    CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)


set(GMOCK_DIR "${CMAKE_SOURCE_DIR}/external/gmock-1.7.0"
    CACHE PATH "The path to the GoogleMock test framework.")

add_subdirectory(external)
add_subdirectory(source)



option(tests_on "Build and run all tests." ON) # Makes boolean 'test' available.
#Turn off with 'cmake -Dtest=OFF'.
if (tests_on)
        enable_testing()
	add_subdirectory(test)
endif()

# This flag is used so that cmake doesn't append the FLAGS every time
# it configures.
set(CONFIGURED_ONCE TRUE CACHE INTERNAL
	"A flag showing that CMake has configured at least once.")
