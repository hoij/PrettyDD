set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")

include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/source")


# add_gmock_test(<target> <lib_to_test> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <lib_to_test>
#  and <sources> and adds the test so that CTest will run it. Both the
#  executable and the test will be named <target>.
function(add_gmock_header_test target)
	add_executable(${target} ${ARGN})
	target_link_libraries(${target} gmock_main)

	add_test(${target} ${target})

	add_custom_command(TARGET ${target}
			   POST_BUILD
			   COMMAND ${target}
			   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			   COMMENT "Running ${target}" VERBATIM)
endfunction()

# add_gmock_test(<target> <lib_to_test> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <lib>
#  and <sources> and adds the test so that CTest will run it. Both the
#  executable and the test will be named <target>.
function(add_gmock_test target lib)
	add_executable(${target} ${ARGN})
	target_link_libraries(${target} gmock_main ${lib})

	add_test(${target} ${target})

	add_custom_command(TARGET ${target}
			   POST_BUILD
			   COMMAND ${target}
			   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			   COMMENT "Running ${target}" VERBATIM)
endfunction()

# add_gmock_test(<target> <lib_to_test> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <lib_to_test>
#  and <sources> and adds the test so that CTest will run it. Both the
#  executable and the test will be named <target>.
function(add_gmock_test_2_libs target lib1 lib2)
	add_executable(${target} ${ARGN})
	target_link_libraries(${target} gmock_main ${lib1} ${lib2})

	add_test(${target} ${target})

	add_custom_command(TARGET ${target}
			   POST_BUILD
			   COMMAND ${target}
			   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			   COMMENT "Running ${target}" VERBATIM)
endfunction()


add_library(parser_param_testlib parser_param_test.cpp formatted_line_stub.h)

#add_gmock_parser_test(parser_chat_test parserlib parser_param_testlib parser_chat_test.cpp)
add_gmock_test_2_libs(parser_meCastNano_test parserlib parser_param_testlib parser_meCastNano_test.cpp)
add_gmock_test_2_libs(parser_health_test parserlib parser_param_testlib parser_health_test.cpp)
add_gmock_test_2_libs(parser_nano_test parserlib parser_param_testlib parser_nano_test.cpp)
add_gmock_test_2_libs(parser_XP_test parserlib parser_param_testlib parser_XP_test.cpp)
add_gmock_test_2_libs(parser_meHitBy_test parserlib parser_param_testlib parser_meHitBy_test.cpp)
add_gmock_test_2_libs(parser_otherAndYourPetHitBy_test parserlib parser_param_testlib parser_otherAndYourPetHitBy_test.cpp)
add_gmock_test_2_libs(parser_misses_test parserlib parser_param_testlib parser_misses_test.cpp)
add_gmock_test_2_libs(parser_system_test parserlib parser_param_testlib parser_system_test.cpp)
add_gmock_test_2_libs(parser_victoryPoints_test parserlib parser_param_testlib parser_victoryPoints_test.cpp)
add_gmock_test_2_libs(parser_youHitOther_test parserlib parser_param_testlib parser_youHitOther_test.cpp)

add_gmock_test(parser_renameSpecial_test parserlib parser_renameSpecial_test.cpp)
add_gmock_test(parser_chat_test parserlib parser_chat_test.cpp)

add_gmock_test(formatted_line_test formatted_linelib formatted_line_test.cpp)
add_gmock_test_2_libs(player_vector_test playerlib loggerlib player_vector_test.cpp)
add_gmock_test(player_test playerlib player_test.cpp)
