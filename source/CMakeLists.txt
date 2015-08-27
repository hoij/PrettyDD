include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)

# CMake cant make libraries out of headers only so they are added here
add_executable(PrettyDD main.cpp
        affected_player_interface.h
        affected_player_vector.h
	base_vector.h
	player_vector.h
	line_info.h
	xp.h
	formatted_line_interface.h
	player_interface.h
	)

# These libraries are mainly made to make test case linking easier
add_library(loggerlib logger.cpp)
add_library(damagelib damage.cpp)
add_library(heallib heal.cpp)
target_link_libraries(heallib loggerlib)
add_library(nanolib nano.cpp)
add_library(nano_programlib nano_program.cpp)
add_library(affected_playerlib affected_player.cpp)
target_link_libraries(affected_playerlib damagelib heallib nanolib)
add_library(playerlib player.cpp)
target_link_libraries(playerlib affected_playerlib nano_programlib damagelib loggerlib)
add_library(formatted_linelib formatted_line.cpp)
target_link_libraries(formatted_linelib loggerlib)
add_library(parserlib parser.cpp)
target_link_libraries(parserlib loggerlib)
add_library(write_stats_to_filelib write_stats_to_file.cpp)
add_library(configurationlib configuration.cpp)

target_link_libraries(PrettyDD
  damagelib
  heallib
  nanolib
  playerlib
  affected_playerlib
  parserlib
  nano_programlib
  loggerlib
  formatted_linelib
  write_stats_to_filelib
  configurationlib
  )
