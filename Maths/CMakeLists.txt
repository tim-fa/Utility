cmake_minimum_required(VERSION 3.10)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)

add_library(Maths STATIC ${SOURCE_FILES})
target_include_directories(Maths PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
