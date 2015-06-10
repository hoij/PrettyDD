cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(PrettyDD)


if(WIN32)
	if(MSVC)
		set(warnings "${CMAKE_CXX_FLAGS} /W4 /WX /EHsc")
	endif()
else()
	set(warnings "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
endif()
if (NOT CONFIGURED_ONCE)
	set(CMAKE_CXX_FLAGS "${warnings}"
	    CACHE STRING "Flags used by the compiler during all build types." FORCE)
	set(CMAKE_C_FLAGS   "${warnings}"
	    CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()





add_subdirectory(source)
add_subdirectory(external)


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
