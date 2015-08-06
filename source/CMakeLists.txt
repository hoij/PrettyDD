include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)

add_executable(PrettyDD main.cpp
        base_player.h
	#player.cpp player.h
	#affected_player.cpp affected_player.h
	player_vector.h
	#parser.cpp parser.h
	#nano_program.cpp nano_program.h
	line_info.h
	heal.h
	damage.h
	xp.h
	nano.h
	formatted_line_interface.h
	#logger.cpp logger.h
	#formatted_line.cpp formatted_line.h
	#write_stats_to_file.cpp write_stats_to_file.h
	#configuration.cpp configuration.h
	)


add_library(playerlib player.cpp)
add_library(affected_playerlib affected_player.cpp)
# CMake cant make libraries out of headers only
#add_library(player_vectorlib player_vector.h)
add_library(loggerlib logger.cpp)
add_library(formatted_linelib formatted_line.cpp)
target_link_libraries(formatted_linelib loggerlib)

#add_library(parserlib
#  parser.cpp
#  formatted_line.cpp
#  line_info.cpp
#  logger.cpp
#  )

add_library(parserlib parser.cpp)
target_link_libraries(parserlib loggerlib)


add_library(nano_programlib nano_program.cpp)
#add_library(heallib heal.h)
#add_library(damagelib damage.h)
#add_library(xplib xp.h)
#add_library(nanolib nano.h)
add_library(write_stats_to_filelib write_stats_to_file.cpp)
add_library(configurationlib configuration.cpp)


target_link_libraries(PrettyDD
  playerlib
  affected_playerlib
  parserlib
  nano_programlib
  #heallib
  #damagelib
  #xplib
  #nanolib
  loggerlib
  formatted_linelib
  write_stats_to_filelib
  configurationlib
  )
