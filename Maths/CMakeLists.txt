cmake_minimum_required(VERSION 3.11)

file(
	GLOB SOURCE_FILES
	*.cpp
)

add_library(Maths STATIC ${SOURCE_FILES})
target_include_directories(Maths PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
