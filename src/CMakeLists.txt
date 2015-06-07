cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

project(PrettyDD)
#set(PrettyDD_VERSION_MAJOR 0)

add_executable(PrettyDD main.cpp
	player.cpp player.h
	parse.cpp parse.h)
