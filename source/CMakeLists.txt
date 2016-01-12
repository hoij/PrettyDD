include_directories(SYSTEM ${GMOCK_DIR}/gtest/include
                           ${GMOCK_DIR}/include)

# CMake cant make libraries out of headers only so they are added here
add_executable(PrettyDD main.cpp
               affected_player_factory.h
               affected_player_factory_interface.h
               affected_player_interface.h
               affected_player_vector.h
               definitions.h
               formatted_line_interface.h
               line_info.h
               log_reading.h
               newline.h
               player_interface.h
               player_factory.h
               player_factory_interface.h)

# These libraries are mainly made to make test case linking easier
add_library(affected_playerlib affected_player.cpp)
target_link_libraries(affected_playerlib damagelib heallib nanolib my_timelib)
add_library(affected_player_vectorlib affected_player_vector.cpp)
target_link_libraries(affected_player_vectorlib affected_playerlib damagelib
                      heallib nanolib loggerlib)
add_library(configurationlib configuration.cpp)
add_library(read_consolelib read_console.cpp)
target_link_libraries(read_consolelib command_handlerlib my_timelib)
add_library(damagelib damage.cpp)
add_library(formatted_linelib formatted_line.cpp)
target_link_libraries(formatted_linelib loggerlib)
add_library(heallib heal.cpp)
target_link_libraries(heallib loggerlib)
add_library(loggerlib logger.cpp)
add_library(my_timelib my_time.cpp)
add_library(nanolib nano.cpp)
add_library(nano_programslib nano_programs.cpp)
add_library(parserlib parser.cpp)
target_link_libraries(parserlib loggerlib)
add_library(playerlib player.cpp)
target_link_libraries(playerlib affected_playerlib nano_programslib damagelib
                      xplib my_timelib affected_player_vectorlib)
add_library(player_vectorlib player_vector.cpp)
target_link_libraries(player_vectorlib playerlib damagelib loggerlib)
add_library(xplib xp.cpp)
target_link_libraries(xplib loggerlib)

# Write to file libs
add_library(writer_helperlib writer_helper.cpp)

add_library(command_handlerlib command_handler.cpp)
target_link_libraries(command_handlerlib damage_writerlib help_writerlib
                      my_timelib nano_program_writerlib playerlib
                      writer_helperlib xp_writerlib player_vectorlib)
add_library(damage_writerlib damage_writer.cpp)
target_link_libraries(damage_writerlib damagelib configurationlib
                      writer_helperlib)
add_library(help_writerlib help_writer.cpp)
target_link_libraries(help_writerlib configurationlib writer_helperlib)
add_library(nano_program_writerlib nano_program_writer.cpp)
target_link_libraries(nano_program_writerlib nano_programslib configurationlib
                      writer_helperlib)
add_library(xp_writerlib xp_writer.cpp)
target_link_libraries(xp_writerlib xplib configurationlib loggerlib
                      writer_helperlib)

target_link_libraries(PrettyDD
                      damagelib
                      read_consolelib
                      heallib
                      nanolib
                      xplib
                      playerlib
                      player_vectorlib
                      affected_playerlib
                      parserlib
                      nano_programslib
                      loggerlib
                      formatted_linelib
                      damage_writerlib
                      nano_program_writerlib
                      xp_writerlib
                      configurationlib
                      command_handlerlib
                      my_timelib
                      help_writerlib)
