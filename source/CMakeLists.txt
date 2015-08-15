include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)

# CMake cant make libraries out of headers only so they are added here
add_executable(PrettyDD main.cpp
        base_player.h
	player_vector.h
	line_info.h
	heal.h
	damage.h
	xp.h
	nano.h
	formatted_line_interface.h
	)

# These libraries are mainly made to make test case linking easier
add_library(nano_programlib nano_program.cpp)
add_library(affected_playerlib affected_player.cpp)
add_library(loggerlib logger.cpp)
add_library(playerlib player.cpp)
target_link_libraries(playerlib nano_programlib affected_player_vectorlib loggerlib)
add_library(formatted_linelib formatted_line.cpp)
target_link_libraries(formatted_linelib loggerlib)
add_library(parserlib parser.cpp)
target_link_libraries(parserlib loggerlib)
add_library(affected_player_vectorlib affected_player_vector.cpp)
target_link_libraries(affected_player_vectorlib affected_playerlib loggerlib)
add_library(write_stats_to_filelib write_stats_to_file.cpp)
add_library(configurationlib configuration.cpp)

target_link_libraries(PrettyDD
  playerlib
  affected_playerlib
  parserlib
  nano_programlib
  loggerlib
  formatted_linelib
  write_stats_to_filelib
  configurationlib
  )
