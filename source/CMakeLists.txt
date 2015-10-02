include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)

# CMake cant make libraries out of headers only so they are added here
add_executable(PrettyDD main.cpp
        affected_player_interface.h
        affected_player_vector.h
	base_vector.h
	player_vector.h
	line_info.h
	formatted_line_interface.h
	player_interface.h
	)

# These libraries are mainly made to make test case linking easier
add_library(my_timelib my_time.cpp)
add_library(loggerlib logger.cpp)
add_library(damagelib damage.cpp)
add_library(heallib heal.cpp)
target_link_libraries(heallib loggerlib)
add_library(nanolib nano.cpp)
add_library(xplib xp.cpp)
target_link_libraries(xplib loggerlib)
add_library(nano_programslib nano_programs.cpp)
add_library(affected_playerlib affected_player.cpp)
target_link_libraries(affected_playerlib damagelib heallib nanolib my_timelib)
add_library(playerlib player.cpp)
target_link_libraries(playerlib affected_playerlib nano_programslib damagelib xplib loggerlib my_timelib)
add_library(formatted_linelib formatted_line.cpp)
target_link_libraries(formatted_linelib loggerlib)
add_library(parserlib parser.cpp)
target_link_libraries(parserlib loggerlib)
add_library(configurationlib configuration.cpp)
add_library(writer_helperlib writer_helper.cpp)
add_library(nano_program_writerlib nano_program_writer.cpp)
target_link_libraries(nano_program_writerlib configurationlib writer_helperlib)
add_library(stat_writerlib stat_writer.cpp)
target_link_libraries(stat_writerlib configurationlib nano_program_writerlib writer_helperlib)
add_library(command_handlerlib command_handler.cpp)

target_link_libraries(PrettyDD
  damagelib
  heallib
  nanolib
  xplib
  playerlib
  affected_playerlib
  parserlib
  nano_programslib
  loggerlib
  formatted_linelib
  stat_writerlib
  configurationlib
  command_handlerlib
  my_timelib
  )
