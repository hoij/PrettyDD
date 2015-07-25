set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")

include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/source")


function(add_gmock_test target lib_to_test)
# add_gmock_test(<target> <lib_to_test> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <lib_to_test>
#  and <sources> and adds the test so that CTest will run it. Both the
#  executable and the test will be named <target>.

	add_executable(${target} ${ARGN})
	target_link_libraries(${target} gmock_main ${lib_to_test})

	add_test(${target} ${target})

	add_custom_command(TARGET ${target}
			   POST_BUILD
			   COMMAND ${target}
			   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			   COMMENT "Running ${target}" VERBATIM)
endfunction()

function(add_gmock_parser_test target lib_to_test helper_lib)
# add_gmock_test(<target> <lib_to_test> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <lib_to_test>
#  and <sources> and adds the test so that CTest will run it. Both the
#  executable and the test will be named <target>.

	add_executable(${target} ${ARGN})
	target_link_libraries(${target} gmock_main ${lib_to_test} ${helper_lib})

	add_test(${target} ${target})

	add_custom_command(TARGET ${target}
			   POST_BUILD
			   COMMAND ${target}
			   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			   COMMENT "Running ${target}" VERBATIM)
endfunction()


add_library(parser_param_testlib parser_param_test.cpp)

#add_gmock_parser_test(parser_chat_test parserlib parser_param_testlib parser_chat_test.cpp)
add_gmock_parser_test(parser_meCastNano_test parserlib parser_param_testlib parser_meCastNano_test.cpp)
add_gmock_parser_test(parser_meGotHealth_test parserlib parser_param_testlib parser_meGotHealth_test.cpp)
add_gmock_parser_test(parser_meGotNano_test parserlib parser_param_testlib parser_meGotNano_test.cpp)
add_gmock_parser_test(parser_meGotSK_test parserlib parser_param_testlib parser_meGotSK_test.cpp)
add_gmock_parser_test(parser_meGotXP_test parserlib parser_param_testlib parser_meGotXP_test.cpp)
add_gmock_parser_test(parser_meHitByEnvironment_test parserlib parser_param_testlib parser_meHitByEnvironment_test.cpp)
add_gmock_parser_test(parser_meHitByMonster_test parserlib parser_param_testlib parser_meHitByMonster_test.cpp)
add_gmock_parser_test(parser_meHitByNano_test parserlib parser_param_testlib parser_meHitByNano_test.cpp)
add_gmock_parser_test(parser_meHitByPlayer_test parserlib parser_param_testlib parser_meHitByPlayer_test.cpp)
add_gmock_parser_test(parser_otherAndYourPetHitByOther_test parserlib parser_param_testlib parser_otherAndYourPetHitByOther_test.cpp)
add_gmock_parser_test(parser_otherHitByNano_test parserlib parser_param_testlib parser_otherHitByNano_test.cpp)
add_gmock_parser_test(parser_otherMisses_test parserlib parser_param_testlib parser_otherMisses_test.cpp)
add_gmock_parser_test(parser_research_test parserlib parser_param_testlib parser_research_test.cpp)
add_gmock_parser_test(parser_system_test parserlib parser_param_testlib parser_system_test.cpp)
add_gmock_parser_test(parser_victoryPoints_test parserlib parser_param_testlib parser_victoryPoints_test.cpp)
add_gmock_parser_test(parser_youGaveHealth_test parserlib parser_param_testlib parser_youGaveHealth_test.cpp)
add_gmock_parser_test(parser_youGaveNano_test parserlib parser_param_testlib parser_youGaveNano_test.cpp)
add_gmock_parser_test(parser_youHitOther_test parserlib parser_param_testlib parser_youHitOther_test.cpp)
add_gmock_parser_test(parser_youHitOtherWithNano_test parserlib parser_param_testlib parser_youHitOtherWithNano_test.cpp)
add_gmock_parser_test(parser_yourMisses_test parserlib parser_param_testlib parser_yourMisses_test.cpp)
add_gmock_parser_test(parser_yourPetHitByMonster_test parserlib parser_param_testlib parser_yourPetHitByMonster_test.cpp)
add_gmock_parser_test(parser_yourPetHitByNano_test parserlib parser_param_testlib parser_yourPetHitByNano_test.cpp)

add_gmock_test(parser_renameSpecial_test parserlib parser_renameSpecial_test.cpp)
add_gmock_test(parser_chat_test parserlib parser_chat_test.cpp)
