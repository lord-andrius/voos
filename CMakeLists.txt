cmake_minimum_required(VERSION 3.20)

project(voo)

add_executable(voo main.c)
IF (NOT WIN32)
	target_link_libraries(voo m)
ENDIF()
