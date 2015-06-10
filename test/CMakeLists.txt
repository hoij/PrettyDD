set(GMOCK_DIR "${CMAKE_SOURCE_DIR}/external/gmock-1.7.0"
    CACHE PATH "The path to the GoogleMock test framework.")


set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")


include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/source)


function(add_gmock_test target)
# add_gmock_test(<target> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <sources> and
#  adds the test so that CTest will run it. Both the executable and the test
#  will be named <target>.

	add_executable(${target} ${ARGN})
	target_link_libraries(${target} gmock_main)

	add_test(${target} ${target})

	add_custom_command(TARGET ${target}
			   POST_BUILD
			   COMMAND ${target}
			   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			   COMMENT "Running ${target}" VERBATIM)
endfunction()

add_gmock_test(tests test1.cpp)

#Link some other files with the test here:
#target_link_libraries(tests player)
