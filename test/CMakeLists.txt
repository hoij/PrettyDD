set_property(TARGET gtest APPEND_STRING PROPERTY COMPILE_FLAGS " -w")

include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/source")



#add_executable(tests test1.cpp)
#target_link_libraries(tests gmock_main)
#
#add_test(tests tests)
#
#add_custom_command(TARGET tests
#		   POST_BUILD
#		   COMMAND tests
#		   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#		   COMMENT "Running tests" VERBATIM)





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

add_gmock_test(parser_otherAndYourPetHitByOther_test parserlib parser_param_test.cpp parser_otherAndYourPetHitByOther_test.cpp)
add_gmock_test(parser_otherHitByNano_test parserlib parser_param_test.cpp parser_otherHitByNano_test.cpp)


#../source/parser.cpp ../source/line_info.cpp ../source/logger.cpp ../source/log_line.cpp ../source/nano_program.cpp ../source/configuration.cpp)




#add_executable(tests parser.cpp)
#target_link_libraries(tests parser)

#Link some other files with the test here:
#target_link_libraries(tests player)
